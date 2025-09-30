#ifndef TASK_H
#define TASK_H

#include "spheretarget.h"

#include <cassert>
#include <vector>

enum class TaskId {
    NONE,
    GRIDSHOT,
};

class Task {
public:
	Task(TaskId tid, Shader& shader) {
		assert(tid != TaskId::NONE);
		taskId = tid;
		if (taskId == TaskId::GRIDSHOT) {
			targets.reserve(10);
			targets.emplace_back(Vector3{8.0f, 0.0f, 0.0f}, 0.3f);
			targets.emplace_back(Vector3{8.0f, 1.0f, 0.0f}, 0.3f);
			targets.emplace_back(Vector3{8.0f, 0.0f, 1.0f}, 0.3f);
			for (int i = 0; i < targets.size(); ++i) {
				targets[i].addShader(shader);
			}
		}
	}
	
	void processHit(SphereTarget& target) {
		if (taskId == TaskId::GRIDSHOT) {
			int newY = (std::rand() % 3) - 1;
			int newZ = (std::rand() % 3) - 1;
			while (targetPresent[newY + 1][newZ + 1]) {
				newY = (std::rand() % 3) - 1;
				newZ = (std::rand() % 3) - 1;
			}
			targetPresent[static_cast<int>(target.position.y + 1.5)]
						 [static_cast<int>(target.position.z + 1.5)] = false;
			target.position.y = newY;
			target.position.z = newZ;
			targetPresent[newY + 1][newZ + 1] = true;
		}
	}
	
	void draw() {
		for (int i = 0; i < targets.size(); ++i) {
			targets[i].draw();
		}
	}
	
	std::vector<SphereTarget> targets{};
	TaskId taskId;
	bool targetPresent[3][3]{
		{0, 1, 0},
		{0, 1, 1},
		{0, 0, 0}
	};
};

#endif