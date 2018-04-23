import gilfullList as gfl
import gillessList as gll

class Test():
	def __init__(self, ltype):
		self.ltype = ltype


	def test_create(self):
		print("Create list test")
		for i in range(5):
			print("Create list " + str(i))
			self.ltype.list_create(4)

		print("Test done!\n")


	def test_append(self):
		print("List append test")
		l = self.ltype.list_create(4)

		for i in range(4):
			print("Set item " + str(i))
			self.ltype.set_item(l, i, 99)

		for i in range(4):
			print("Appending " + str(i))
			self.ltype.list_append(l, 99)

		for i in range(8):
			print("Getting item " + str(i))
			print(self.ltype.get_item(l, i))

		print("The list is: ")
		print(l)
		print("Test done!\n")



def main():
	print("Testing gilfullList")
	testgfl = Test(gfl)
	testgfl.test_create()
	testgfl.test_append()
	print("Testing gillessList")	
	testgll = Test(gll)
	testgll.test_create()
	testgll.test_append()


if __name__ == '__main__':
	main()
