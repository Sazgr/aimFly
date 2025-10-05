#ifndef TASK_H
#define TASK_H

#include "target.h"

#include <cassert>
#include <vector>

enum class TaskId {
    NONE,
    GRIDSHOT,
	STRAFESHOT,
	HEADSHOT,
	PEEKSHOT,
};

class Task {
public:
	Task(TaskId tid, Shader& shader) {
		assert(tid != TaskId::NONE);
		taskId = tid;
		targets.reserve(10);
		if (taskId == TaskId::GRIDSHOT) {
			targets.emplace_back(TargetType::SPHERE, Vector3{8.0f, 0.0f, 0.0f}, 0.3f);
			targets.emplace_back(TargetType::SPHERE, Vector3{8.0f, 1.0f, 0.0f}, 0.3f);
			targets.emplace_back(TargetType::SPHERE, Vector3{8.0f, 0.0f, 1.0f}, 0.3f);
		} else if (taskId == TaskId::STRAFESHOT) {
			targets.emplace_back(TargetType::SPHERE, Vector3{8.0f, 0.0f, 0.0f}, 0.4f);
			targets[0].velocity = Vector3{0, 0, 0.02f};
		} else if (taskId == TaskId::HEADSHOT) {
			targets.emplace_back(TargetType::BODY, Vector3{20.0f, 0.0f, 0.0f}, 0.3f);
			targets[0].velocity = Vector3{0, 0, 0.02f};
		}
		for (int i = 0; i < targets.size(); ++i) {
			targets[i].addShader(shader);
		}
	}
	
	void tick() {
		if (taskId == TaskId::GRIDSHOT) {
			; //no processing needed
		} else if (taskId == TaskId::STRAFESHOT) {
			int turnChance = 50;
			if ((targets[0].position.z > 4 && targets[0].velocity.z > 0) || (targets[0].position.z < -4 && targets[0].velocity.z < 0)) {
				turnChance = 25 - 15 * abs(targets[0].position.z) >= 5; //target heading away from center
			}
			if ((targets[0].position.z > 4 && targets[0].velocity.z < 0) || (targets[0].position.z < -4 && targets[0].velocity.z > 0)) {
				turnChance = 100; //target going back to center
			}
			if (std::rand() % 30 == 0) {
				targets[0].velocity.z = -targets[0].velocity.z;
			}
			targets[0].position = Vector3Add(targets[0].position, targets[0].velocity);
		} else if (taskId == TaskId::HEADSHOT) {
			int turnChance = 50;
			if ((targets[0].position.z > 4 && targets[0].velocity.z > 0) || (targets[0].position.z < -4 && targets[0].velocity.z < 0)) {
				turnChance = 25 - 15 * abs(targets[0].position.z) >= 5; //target heading away from center
			}
			if ((targets[0].position.z > 4 && targets[0].velocity.z < 0) || (targets[0].position.z < -4 && targets[0].velocity.z > 0)) {
				turnChance = 100; //target going back to center
			}
			if (std::rand() % 30 == 0) {
				targets[0].velocity.z = -targets[0].velocity.z;
			}
			targets[0].position = Vector3Add(targets[0].position, targets[0].velocity);
		}
	}
	
	bool processHit(Target& target, int hitType, int& score) {
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
			return true;
		} else if (taskId == TaskId::STRAFESHOT) {
			int newZ = (std::rand() % 7) - 3;
			target.position.z = newZ;
			return true;
		} else if (taskId == TaskId::HEADSHOT) {
			target.health -= (hitType == HEADSHOT ? 4 : 1);
			if (target.health <= 0) {
				target.health = 4;
				int newZ = (std::rand() % 7) - 3;
				target.position.z = newZ;
				return true;
			}
			return false;
		} else {
			assert(false);
		}
	}
	
	void draw() {
		for (int i = 0; i < targets.size(); ++i) {
			targets[i].draw();
		}
	}
	
	std::vector<Target> targets{};
	TaskId taskId;
	bool targetPresent[3][3]{
		{0, 1, 0},
		{0, 1, 1},
		{0, 0, 0}
	};
};

#endif