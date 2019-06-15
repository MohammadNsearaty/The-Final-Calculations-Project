#pragma once
#ifndef INTERVAL_H
#define INTERVAL_H
class Interval
{
private:
	float min;
	float max;

public:
	Interval()
	{
		max = min = 0.0f;
	}
	Interval(float min, float max)
	{
		this->min = min;
		this->max = max;
	}
	Interval(const Interval &interval)
	{
		this->min = interval.min;
		this->max = interval.max;
	}
	void setMin(float min) { this->min = min; }
	void setMax(float max) { this->max = max; }
	float getMin() { return this->min; }
	float getMax() { return this->max; }
};

#endif