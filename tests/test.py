from gillessList import *
import gc

class Test():

	def __init__(self):
		gc.disable()

	def test_create(self):
		print("Create list test")
		for i in range(5):
			print("Create list " + str(i))
			list_create(4)

		print("Test done!\n")


	def test_append(self):
		print("List append test")
		l = list_create(4)

		for i in range(4):
			print("Set item " + str(i))
			set_item(l, i, i)

		for i in range(4):
			print("Appending " + str(i))
			list_append(l, 99)


		print("Test done!\n")

def main():
	test = Test()
	
	test.test_create()

	# test.test_append()

if __name__ == '__main__':
	main()