import gilfullList as gfl
import gillessList as gll
import time
import threading
import gc
import sys


class Test():
	def __init__(self, ltype, numObjects=50):
		self.ltype = ltype
		self.numObjects = numObjects


	def test_create(self):	
		for i in range(50):
			self.ltype.list_create(4)
		

	def test_setget(self):
		
		l = self.ltype.list_create(self.numObjects)
		
		for i in range(self.numObjects):
			self.ltype.set_item(l, i, 99)
		
		
		for i in range(self.numObjects):
			self.ltype.get_item(l, i)
		


	def test_append(self):
		
		l = self.ltype.list_create(4)
		for i in range(self.numObjects):
			self.ltype.list_append(l, 99)
		
		
	

def single_threaded_test(obj):
	
	start = time.time()
	obj.test_create()
	end = time.time()
	create_list_time = end - start
	print("Time taken for create 50 lists: " + str(create_list_time))

	print("In set & get test")
	start = time.time()
	obj.test_setget()
	end = time.time()
	set_get_time = end - start
	print("Time taken for set and get " + str(obj.numObjects) + " objects: " + str(set_get_time))

	print("List append test")
	start = time.time()
	obj.test_append()
	end = time.time()
	append_time = end - start
	print("Time taken for appending " + str(obj.numObjects) + " objects: " + str(append_time))

	return (create_list_time, set_get_time, append_time)


def multi_threaded_test(obj, numThreads):
	print("Create list test")
	create_list_time = 0
	threads = {}
	for i in range(numThreads):
		t = threading.Thread(target=obj.test_create())
		threads[t] = time.time()
		t.start()
		
	for t in threads:
		t.join()
		create_list_time += (time.time() - threads[t]) / numThreads
	print("Time taken for create 50 lists: " + str(create_list_time))

	print("In set & get test")
	set_get_time = 0
	threads = {}
	for i in range(numThreads):
		t = threading.Thread(target=obj.test_setget())
		threads[t] = time.time()
		t.start()
		
	for t in threads:
		t.join()
		set_get_time += (time.time() - threads[t]) / numThreads
	print("Time taken for set and get " + str(obj.numObjects) + " objects: " + str(set_get_time))

	print("List append test")
	append_time = 0
	threads = {}
	for i in range(numThreads):
		t = threading.Thread(target=obj.test_append())
		threads[t] = time.time()
		t.start()
		
	for t in threads:
		t.join()
		append_time += (time.time() - threads[t]) / numThreads
	print("Time taken for appending " + str(obj.numObjects) + " objects: " + str(append_time))

	return (create_list_time, set_get_time, append_time)
		

def main():

	gc.disable()

	create_data_file = open("create_data.csv", "w")
	set_get_data_file = open("set_get_data.csv", "w")
	append_data_file = open("append_data.csv", "w")

	for i in range(0, 80, 10):
		testgfl = Test(gfl, i)
		testgll = Test(gll, i)

		gfl_create_avg = 0
		gfl_get_set_avg = 0
		gfl_append_avg = 0

		gll_create_avg = 0
		gll_get_set_avg = 0
		gll_append_avg = 0

		for j in range(50):

	
			print("Testing gilfullList for single threaded ")
			create_list_time, set_get_time, append_time = single_threaded_test(testgfl)
			gfl_create_avg += create_list_time
			gfl_get_set_avg += set_get_time
			gfl_append_avg += append_time


			print("\n\nTesting gillessList for single threaded  ")	
			create_list_time, set_get_time, append_time = single_threaded_test(testgll)
			gll_create_avg += create_list_time
			gll_get_set_avg += set_get_time
			gll_append_avg += append_time


			# print("\n\nTesting gilfullList for multi threaded ")	
			# create_list_time, set_get_time, append_time = multi_threaded_test(testgfl, 3)
			# gfl_create_avg += create_list_time
			# gfl_get_set_avg += set_get_time
			# gfl_append_avg += append_time
		    		

			# print("\n\nTesting gillessList for multi threaded  ")	
			# create_list_time, set_get_time, append_time = multi_threaded_test(testgll, 3)
			# gll_create_avg += create_list_time
			# gll_get_set_avg += set_get_time
			# gll_append_avg += append_time

		gfl_create_avg /= 50
		gfl_get_set_avg /= 50
		gfl_append_avg /= 50

		gll_create_avg /= 50
		gll_get_set_avg /= 50
		gll_append_avg /= 50

		
		create_data_file.write(str(i) + "," + str(gll_create_avg) + "," + str(gfl_create_avg) + "\n")
		set_get_data_file.write(str(i) + "," + str(gll_get_set_avg) + "," + str(gfl_get_set_avg) + "\n")
		append_data_file.write(str(i) + "," + str(gll_append_avg) + "," + str(gfl_append_avg) + "\n")

	create_data_file.close()
	set_get_data_file.close()
	append_data_file.close()
	


if __name__ == '__main__':
	main()
