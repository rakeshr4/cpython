import gilfullList as gfl
import gillessList as gll
import time
import threading


class Test():
	def __init__(self, ltype):
		self.ltype = ltype


	def test_create(self):	
		print("Create list test")		
		for i in range(50):
			self.ltype.list_create(4)

	def test_setget(self):
		
		l = self.ltype.list_create(50)
		
		for i in range(50):
			self.ltype.set_item(l, i, 99)
		
		for i in range(50):
			self.ltype.get_item(l, i)


	def test_append(self):
		
		l = self.ltype.list_create(4)
		for i in range(50):
			self.ltype.list_append(l, 99)
		
		
		
def main():
	print("Testing gilfullList for single threaded ")
	testgfl = Test(gfl)

	#print("Create list test")
	start = time.time()
	testgfl.test_create()
	end = time.time()
	total = end - start
	print("Time taken for create 50 lists: " + str(total))

	print("In set & get test")
	start = time.time()
	testgfl.test_setget()
	end = time.time()
	total = end - start
	print("Time taken for set and get 50 objects: " + str(total))

	print("List append test")
	start = time.time()
	testgfl.test_append()
	end = time.time()
	total = end - start
	print("Time taken for appending 50 objects: " + str(total))

	print("\n\nTesting gillessList for single threaded  ")	
	testgll = Test(gll)

	print("Create list test")
	start = time.time()
	testgll.test_create()
	end = time.time()
	total = end - start
	print("Time taken for create 50 lists: " + str(total))

	print("In set & get test")
	start = time.time()
	testgll.test_setget()
	end = time.time()
	total = end - start
	print("Time taken for set and get 50 objects: " + str(total))

	print("List append test")
	start = time.time()
	testgll.test_append()
	end = time.time()
	total = end - start
	print("Time taken for appending 50 objects: " + str(total))


	print("\n\nTesting gilfullList for multi threaded ")
	testgfl = Test(gfl)
	

	print("Create list test")
	start = time.time()
	for i in range(3):
		t1 = threading.Thread(target=testgfl.test_create())
		t1.daemon=True
		t1.start()
	end = time.time()
	total = end - start
	print("Time taken for create 50 lists: " + str(total))

	print("In set & get test")
	start = time.time()
	for i in range(3):
		t2 = threading.Thread(target=testgfl.test_setget())
		t2.daemon=True
		t2.start()
	end = time.time()
	total = end - start
	print("Time taken for set and get 50 objects: " + str(total))

	print("List append test")
	start = time.time()
	for i in range(3):
		t3 = threading.Thread(target=testgfl.test_setget())
		t3.daemon=True
		t3.start()
	end = time.time()
	total = end - start
	print("Time taken for appending 50 objects: " + str(total))
    
	print("\n\nTesting gillessList for multi threaded  ")	
	testgll = Test(gll)
	print("Create list test")
	start = time.time()
	for i in range(3):
		t1 = threading.Thread(target=testgll.test_create())
		t1.daemon=True
		t1.start()
	end = time.time()
	total = end - start
	print("Time taken for create 50 lists: " + str(total))

	print("In set & get test")
	start = time.time()
	for i in range(3):
		t2 = threading.Thread(target=testgll.test_setget())
		t2.daemon=True
		t2.start()
	end = time.time()
	total = end - start
	print("Time taken for set and get 50 objects: " + str(total))

	print("List append test")
	start = time.time()
	for i in range(3):
		t3 = threading.Thread(target=testgll.test_setget())
		t3.daemon=True
		t3.start()
	end = time.time()
	total = end - start
	print("Time taken for appending 50 objects: " + str(total))
	


if __name__ == '__main__':
	main()
