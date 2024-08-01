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
print("Imported!")

class TestReward(RewardFunction):
	def get_reward(self, player, state):
		return 0

class TestPolicyInterface(PolicyInterface):
	def get_actions(self, all_obs):
		print("received observations:", all_obs)
		return np.array([0, 0])

def create_env_fn():
	
	config = EnvConfig()
	config.reward_fn = CombinedReward([
		(VelocityPlayerToBallReward(), 1.0),
		(FaceBallReward(), 0.1),
		(TestReward(), 0.2)
	])

	config.state_setter = KickoffState()
	config.terminal_conds = [
		NoTouchCondition(10)
	]
	config.action_parser = DefaultAction()
	config.obs_builder = DefaultObs()
	return config

def proc_main():
	interface = TestPolicyInterface()
	rapid_learn.collector.run(create_env_fn, interface)

def main():
	try:
		processes = []
		for i in range(5):
			p = Process(target=proc_main, args=())
			p.start()
		print("Processes started!")
		print("Done!")
	except Exception as e:
		print("EXCEPTION:", traceback.format_exc())
	
if __name__ == '__main__':
	freeze_support()
	main()
	