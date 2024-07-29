print("Importing...")
import rapid_learn
from rapid_learn import *
from rapid_learn.rewards import *
from rapid_learn.state_setter import *
import traceback
print("Imported!")

class TestReward(RewardFunction):
	def get_reward(self, player, state):
		print("GetReward", player, state)
		return 0

#class Cat(Animal):
#	def go_py(self, n_times):
#		return "meow! " * n_times

def env_create_fn():

	# This all works
	#c = Cat()
	#print(call_go(c))
	#print(c.go_py(10))

	print("Making reward...")
	config = rapid_learn.EnvConfig()
	reward_fn = TestReward()
	print("Setting to config...")
	config.reward_fn = reward_fn
	config.reward_fn = VelocityReward()
	print("Resetting...")	
	reward_fn.reset(GameState())
	print("Getting reward...")
	reward_fn.get_reward(PlayerData(), GameState())
	print("Done")
	return config

try:
	rapid_learn.collector.run(env_create_fn)
except Exception as e:
	print("EXCEPTION:", traceback.format_exc())
	