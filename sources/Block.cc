#include "Block.hh"

bool Block::multithreading(true);

void Block::Update(uint32_t time)
{
	visit_timestamp = time;
	logger->Enter(name+" Block Update");
	bool need_to_compute = false;
	for (auto in: inputs)
	{
		in->Update(time);
		if (in->GetChangeTimestamp() > computation_timestamp)
			need_to_compute = true;
	}
	if (need_to_compute)
	{
		for (auto out: outputs)
			out->Initialize();
		if (multithreading)
			fut = std::async(std::launch::async, &Block::Compute, this);
		else
			Compute();
		computation_timestamp = time;
	}
	logger->Exit();
}

void Block::ManualUpdate(uint32_t time)
{
	visit_timestamp = time;
	logger->Enter(name+" Block Update");
	bool need_to_compute = false;
	for (auto in: inputs)
	{
		in->Update(time);
		if (in->GetChangeTimestamp() > computation_timestamp)
			need_to_compute = true;
	}
	if (need_to_compute)
	{
		for (auto out: outputs)
			out->Initialize();
		Compute();
		computation_timestamp = time;
	}
	logger->Exit();
}
