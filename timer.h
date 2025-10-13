#ifndef TIMER_H
#define TIMER_H

#include <atomic>
#include <chrono>

class Timer {
private:
	using clock_t = std::chrono::steady_clock;
	using second_t = std::chrono::duration<double, std::ratio<1>>;	
	std::chrono::time_point<clock_t> m_beg;
	std::chrono::time_point<clock_t> m_lastFrame;
	double m_deltaTime = 0.0;
public:
	Timer() : m_beg(clock_t::now()), m_lastFrame(clock_t::now()) {}
	
	void reset() {
		m_beg = clock_t::now();
		m_lastFrame = clock_t::now();
		m_deltaTime = 0.0;
	}
	
	double elapsed() const {
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}
	
	double updateDeltaTime() {
		auto now = clock_t::now();
		m_deltaTime = std::chrono::duration_cast<second_t>(now - m_lastFrame).count();
		m_lastFrame = now;
		return m_deltaTime;
	}
	
	double getDeltaTime() const {
		return m_deltaTime;
	}
};

#endif