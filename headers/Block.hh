#pragma once
#include <future>
#include <string>
#include <algorithm>
#include "Logger/Logger.hh"

class
{
private:
	uint32_t time;
public:
	uint32_t operator()()
	{
		return ++time;
	}
} GlobalTimestamp;

template <typename Type> class Input;
template <typename Type> class Output;
class Block;
template <typename Type> class ManualInput;

class InputMeta
{
	friend class Block;
protected:
	virtual void Update(uint32_t time) = 0;
	virtual uint32_t GetChangeTimestamp() = 0;
};

template <typename Type>
class Input: public InputMeta
{
private:
	Output<Type>* out;
	uint32_t connection_timestamp;
	Block* parent;
protected:
	virtual void Update(uint32_t time) override;
	virtual uint32_t GetChangeTimestamp() override;
public:
	void Connect(Output<Type>* _out);
	Input(Block* _parent);
	Input(const Input& other) = delete;
	Type GetData();
};

class OutputMeta
{
	friend class Block;
protected:
	virtual void Initialize() = 0;
};

class Block
{
	template <typename Type>
	friend class Input;
	template <typename Type>
	friend class ManualInput;
	template <typename Type>
	friend class Output;
private:
	std::vector <InputMeta*> inputs;
	std::vector <OutputMeta*> outputs;
	std::future <void> fut;
	uint32_t visit_timestamp;
	uint32_t computation_timestamp;
protected:
	std::string name;
	Logger* logger;
	virtual void Compute() = 0;
	template <typename Type>
	void SetData(Output<Type>* _out, Type data)
	{
		if (_out->parent != this)
		{
			logger->Log(Logger::LogLevel::Error)<<"Writing on other's output";
			throw "Writing on other's output";
		}
		_out->SetData(data);
	}
	void Update(uint32_t time = GlobalTimestamp());
public:
	void ManualUpdate(uint32_t time = GlobalTimestamp());
	static bool multithreading;
	Block(std::string _name, Logger* _logger)
	: name(_name)
	, logger(_logger)
	, visit_timestamp(0)
	, computation_timestamp(0)
	{};
	Block (const Block& other) = delete;
};

template <typename Type>
class Output: public OutputMeta
{
	friend class Block;
	template <typename TType>
	friend class Input;
private:
	Block* parent;
	std::promise<Type>* data_promise;
	std::future<Type> future;
	std::shared_future<Type> shared_future;
protected:
	virtual void Update(uint32_t time);
	void SetData(Type t);
	virtual void Initialize() override;
	virtual uint32_t GetVisitTimestamp()
	{
		return parent->visit_timestamp;
	}
public:
	Output(Block* _parent);
	Output(const Output& other) = delete;
};

template <typename Type>
class ManualInput: public Output<Type>
{
private:
	uint32_t visit_timestamp;
protected:
	virtual void Update(uint32_t time) override
	{
		visit_timestamp = time;
	}
	virtual uint32_t GetVisitTimestamp() override
	{
		return visit_timestamp;
	}
public:
	void SetDataManually(Type t)
	{
		Output<Type>::Initialize();
		Output<Type>::SetData(t);
	}
	ManualInput()
	: Output<Type>(nullptr)
	{}
	ManualInput(Type t)
	: Output<Type>(nullptr)
	{
		SetDataManually(t);
	}
	ManualInput(const ManualInput& other) = delete;
};

template <typename Type>
void Input<Type>::Update(uint32_t time)
{
	if (out == nullptr)
	{
		parent->logger->Log(Logger::LogLevel::Error)
			<<"Updating input not connected to any output";
		throw "Updating input not connected to any output";
	}
	if (out->GetVisitTimestamp() < time)
	{
		out->Update(time);
	}
}

template <typename Type>
uint32_t Input<Type>::GetChangeTimestamp()
{
	if (out-> parent != nullptr)
		return std::max(connection_timestamp,
			out->parent->computation_timestamp);
	return connection_timestamp;
}

template <typename Type>
void Input<Type>::Connect(Output<Type>* _out)
{
	out = _out;
	connection_timestamp = GlobalTimestamp();
}

template <typename Type>
Input<Type>::Input(Block* _parent)
{
	parent = _parent;
	out = nullptr;
	parent->inputs.push_back(this);
	connection_timestamp = 0;
}

template <typename Type>
Type Input<Type>::GetData()
{
	if (out == nullptr)
	{
		parent->logger->Log(Logger::LogLevel::Error)
			<<"getting data from input not connected to any output";
		throw "getting data from input not connected to any output";
	}
	return out->shared_future.get();
}

template <typename Type>
void Output<Type>::Update(uint32_t time)
{
	parent->Update(time);
}

template <typename Type>
void Output<Type>::Initialize()
{
	if (data_promise != nullptr)
		delete data_promise;
	data_promise = new std::promise<Type>;
	future = data_promise->get_future();
	shared_future = future.share();
}

template <typename Type>
void Output<Type>::SetData(Type t)
{
	data_promise->set_value(t);
}

template <typename Type>
Output<Type>::Output(Block* _parent)
{
	parent = _parent;
	if (parent != nullptr)
		parent->outputs.push_back(this);
	data_promise = nullptr;
}
