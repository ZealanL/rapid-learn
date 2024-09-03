import torch

print("Importing...")
import rapid_learn
from rapid_learn import *
from rapid_learn.rewards import *
from rapid_learn.states import *
from rapid_learn.obs import *
from rapid_learn.acts import *
from rapid_learn.terminals import *
import inspect
import traceback
import wandb
import numpy as np
#from torch.multiprocessing import Process, Pool, Queue
from multiprocessing import Process, Pool, Queue
from threading import Thread
#import torch
#from torch import nn
import time
from ppo.ppo_learner import PPOLearner
print("Imported!")

class TestReward(RewardFunction):
	def get_reward(self, player, state):
		return 0

class TestPolicyInterface(PolicyInterface):
	def get_actions(self, all_obs):
		return np.array([0, 0])

def create_env_fn():
	
	config = EnvConfig()
	config.reward_fn = CombinedReward([
		(VelocityReward(), 1),
		(TestReward(), 0.2)
	])

	config.state_setter = RandomState(True, True, False)
	config.terminal_conds = [
		NoTouchCondition(10)
	]
	config.action_parser = DefaultAction()
	config.obs_builder = DefaultObs()
	return config

NUM_THREADS = 1
def proc_main():
	interface = TestPolicyInterface()
	collector = rapid_learn.Collector(create_env_fn, interface)
	collected_exp = collector.collect(100)
def main():
	'''
	try:
		processes = []
		for i in range(NUM_THREADS):
			p = Thread(target=proc_main, args=())
			p.start()
		print("Processes started!")
	except Exception as e:
		print("EXCEPTION:", traceback.format_exc())
	'''
	batch_size = 10000
	obs_size = 89
	num_actions = 90
	ppo_inst = PPOLearner(obs_size, num_actions, batch_size)

	class PPOPolicyInterface(PolicyInterface):
		def __init__(self):
			super().__init__()
			self.log_probs = []

		def reset(self):
			self.log_probs = []

		def get_actions(self, all_obs):
			with torch.no_grad():
				actions, log_probs = ppo_inst.policy.get_stochastic_action(all_obs)
				self.log_probs.append(log_probs)
				return actions.cpu().numpy()

	interface = PPOPolicyInterface()
	collector = rapid_learn.Collector(create_env_fn, interface)
	while True:
		report = {}

		interface.reset()
		collected_exp = collector.collect(batch_size//2)
		states = np.array(collected_exp.obs).reshape(-1, obs_size)
		actions = np.array(collected_exp.actions).flatten()
		log_probs = torch.stack(interface.log_probs).flatten()
		rewards = np.array(collected_exp.rewards).flatten()
		dones = np.array(collected_exp.dones).flatten()

		truncateds = np.zeros(np.shape(collected_exp.dones))
		truncateds[..., -1:] = 1
		truncateds = truncateds.flatten()
		ppo_inst.learn(states, actions, log_probs, rewards, dones, truncateds, report)

		print("===============")
		for k,v in report.items():
			print(f"{k}: {v}")


	#interface =
	
if __name__ == '__main__':
	main()
	