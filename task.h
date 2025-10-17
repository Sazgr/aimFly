#ifndef TASK_H
#define TASK_H

#include "worldobject.h"
#include "target.h"

#include <cassert>
#include <vector>

constexpr float targetSpeed = 3.0f;

enum class TaskId {
    NONE,
    GRIDSHOT,
	STRAFESHOT,
	HEADSHOT,
	BOXSHOT,
};

class Task {
public:
	Task(TaskId tid, Shader& shader) {
		setTask(tid, shader);
	}
	
	void setTask(TaskId tid, Shader& shader) {
		assert(tid != TaskId::NONE);
		
		taskId = tid;
		targets.clear();
		targets.reserve(10);
		objects.clear();
		objects.reserve(50);
		
		if (taskId == TaskId::GRIDSHOT) {
			targets.emplace_back(TargetType::SPHERE, Vector3{8.0f, 0.0f, 0.0f}, 0.3f);
			targets.emplace_back(TargetType::SPHERE, Vector3{8.0f, 1.0f, 0.0f}, 0.3f);
			targets.emplace_back(TargetType::SPHERE, Vector3{8.0f, 0.0f, 1.0f}, 0.3f);
		} else if (taskId == TaskId::STRAFESHOT) {
			targets.emplace_back(TargetType::SPHERE, Vector3{8.0f, 0.0f, 0.0f}, 0.4f);
			targets[0].velocity = Vector3{0, 0, targetSpeed};
		} else if (taskId == TaskId::HEADSHOT) {
			targets.emplace_back(TargetType::BODY, Vector3{20.0f, 0.0f, 0.0f}, 0.3f);
			targets[0].velocity = Vector3{0, 0, targetSpeed};
		} else if (taskId == TaskId::BOXSHOT) {
			targets.emplace_back(TargetType::BODY, Vector3{20.0f, 0.0f, 0.0f}, 0.3f);
			targets[0].velocity = Vector3{0, 0, targetSpeed};
			
			objects.emplace_back(Vector3{15.0f, -1.0f, 0.0f}, 2, 3, 2, true);
		}
		for (int i = 0; i < targets.size(); ++i) {
			targets[i].addShader(shader);
		}
		for (int i = 0; i < objects.size(); ++i) {
			objects[i].addShader(shader);
		}
	}
	
	void resetTimer() {
		time = -3.0;
	}
	
	void tickPaused(double deltaTime) {
		time += deltaTime;
	}
	
	void tick(double deltaTime) {
		time += deltaTime;
		
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
			targets[0].position = Vector3Add(targets[0].position, Vector3Scale(targets[0].velocity, (float)deltaTime));
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
			targets[0].position = Vector3Add(targets[0].position, Vector3Scale(targets[0].velocity, (float)deltaTime));
		} else if (taskId == TaskId::BOXSHOT) {
			if (abs(targets[0].position.z) > 3) {
				targets[0].velocity.z = -targets[0].velocity.z;
			}
			targets[0].position = Vector3Add(targets[0].position, Vector3Scale(targets[0].velocity, (float)deltaTime));
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
		} else if (taskId == TaskId::BOXSHOT) {
			target.health -= (hitType == HEADSHOT ? 4 : 1);
			if (target.health <= 0) {
				target.health = 4;
				target.position.z = 0;
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
		for (int i = 0; i < objects.size(); ++i) {
			objects[i].draw();
		}
	}
	
	std::vector<Target> targets{};
	std::vector<WorldObject> objects{};
	TaskId taskId;
	bool targetPresent[3][3]{
		{0, 1, 0},
		{0, 1, 1},
		{0, 0, 0}
	};
	double time{0.0};
};

#endif