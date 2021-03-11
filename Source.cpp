#include <iostream>
#include <fstream>
#include <string>
#include <limits>
//#include <bits/stdc++.h>
#include <math.h>
#include <time.h>

using namespace std;


// i used template to create a generic list structure
// so we can hold string, int or any other data type as our data in the Node object
template <class T>
struct Node
{
	T data;
	Node* next;
};

template<class T>
class List
{
private:
	Node<T>* head;
public:
	List() { head = NULL; }
	void insert(T str);
	void clear();
	void show();
};

template <class T>
void List<T>::insert(T str) {
	Node<T>* newNode = new Node<T>();
	newNode->data = str;
	newNode->next = NULL;
	if (head == NULL) {
		head = newNode;
	}
	else {
		Node<T>* traverse = head;
		while (traverse->next) {
			traverse = traverse->next;
		}
		traverse->next = newNode;
	}
}

template <class T>
void List<T>::clear() {
	Node<T>* traverse = head;
	Node<T>* target;
	while (traverse) {
		target = traverse;
		traverse = traverse->next;
		delete target;
	}
}

template <class T>
void List<T>::show() {
	Node<T>* traverse = head;
	while (traverse) {
		cout << traverse->data;
		traverse = traverse->next;
		if (traverse)
			cout << ", ";
	}
	
	cout << endl;
}

class Employee
{
public:
	int id;					
	int totalCalls = 0;
	int performanceScore = 0;	// will be the key parameter of the heap
};

class Heap
{
private:
	int capacity;
	int size;
	Employee* arr;
	int parentIndex(int i) { return i/2; }
	int leftIndex(int i) { return 2 * i; }
	int rightIndex(int i) { return 2 * i + 1; }

	void swapEmployees(int i, int j);	// i and j are index of arr array
	void increaseCapacity();			// doubles the capacity of the array
	
	void maxHeapifyByTotalCalls(int n, int i);
public:
	Heap();
	Heap(int cap);
	//~Heap();
	void maxHeapify(int n, int i);	//	i: root index 
									//	n: index of the last element in the tree
	void buildMaxHeap();
	void heapSort();

	void heapSortByTotalCalls();

	void increaseKey(int i, int key, int totalCalls);	// sets the performance of the employee with index i to "key" and reorganize the heap
	Employee extractMax();
	void insert(Employee e);
	

	int fillArrayFromFile(string fileName);
	void displayEndOfDayResults(int day);
	Employee getEmployeebyIndex(int i) { return arr[i]; }
	int findEmployeeIndexById(int id); // returns -1 if it doesnt exist
	int getHeight() { return (int) ceil(log2(size + 1)) -  1; }

	void show(int n) { for (int i = 0; i < n; i++){ cout << arr[i].id << "\t" << arr[i].performanceScore << endl << endl; } } // TODO: remove this
};

void Heap::swapEmployees(int i, int j) {
	Employee temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;
}

void Heap::increaseCapacity() {
	Employee* newArr = new Employee[capacity * 2];
	for (int i = 0; i < capacity; i++)
		newArr[i] = arr[i];
	capacity *= 2;
	delete [] arr;
	arr = newArr;
}

int Heap::findEmployeeIndexById(int id) {
	for (int i = 0; i < size; i++) {
		if (arr[i].id == id) {
			return i;
		}
	}
	return -1;
}

Heap::Heap() {
	capacity = 1;
	size = 0;
	arr = new Employee[capacity];
}

Heap::Heap(int cap) {
	capacity = cap;
	size = 0;
	arr = new Employee[capacity];
}

void Heap::maxHeapify(int n, int i) { // n is last index 
	int l = leftIndex(i);
	int r = rightIndex(i);
	int largestIndex = i;

	if (l <= n && arr[l].performanceScore > arr[i].performanceScore) {
		largestIndex = l;
	}
	else {
		largestIndex = i;	// no need to this else 
	}
	if (r <= n && arr[r].performanceScore > arr[largestIndex].performanceScore) {
		largestIndex = r;
	}

	if (largestIndex != i) {
		swapEmployees(i, largestIndex);
		maxHeapify(n, largestIndex);
	}
}

void Heap::buildMaxHeap() {
	for (int i = (size - 1) / 2; i >= 0; i--) {
		maxHeapify(size - 1, i);
	}
}

void Heap::heapSort() {
	buildMaxHeap();
	for (int i = size - 1; i >= 1; i--) {
		swapEmployees(0, i);
		maxHeapify(i - 1, 0);
	}
}


void Heap::maxHeapifyByTotalCalls(int n, int i) { // n is last index 
	int l = leftIndex(i);
	int r = rightIndex(i);
	int largestIndex = i;

	if (l <= n && arr[l].totalCalls > arr[i].totalCalls) {
		largestIndex = l;
	}
	else {
		largestIndex = i;	// no need to this else 
	}
	if (r <= n && arr[r].totalCalls > arr[largestIndex].totalCalls) {
		largestIndex = r;
	}

	if (largestIndex != i) {
		swapEmployees(i, largestIndex);
		maxHeapify(n, largestIndex);
	}
}

void Heap::heapSortByTotalCalls() {
	for (int i = (size - 1) / 2; i >= 0; i--) {
		maxHeapifyByTotalCalls(size - 1, i);
	}
	for (int i = size - 1; i >= 1; i--) {
		swapEmployees(0, i);
		maxHeapifyByTotalCalls(i - 1, 0);
	}
}


void Heap::increaseKey(int i, int key, int totalCalls) {
	if (key < arr[i].performanceScore) {
		cout << "new key is smaller than current key" << endl;
		return;
	}
	arr[i].performanceScore = key;
	arr[i].totalCalls = totalCalls;
	while (i > 0 && arr[parentIndex(i)].performanceScore < arr[i].performanceScore) { // TODO: check i > 0 if correct
		swapEmployees(i, parentIndex(i));
		i = parentIndex(i);
	}
}

Employee Heap::extractMax() {
	if (size < 1) {
		cout << "heap underflow" << endl;
		return *(new Employee());	//		TODO: handle the error in more proper way
	}
	Employee e = arr[0];
	arr[0] = arr[size - 1];
	//delete (arr + size - 1);		// remove this
	size--;
	maxHeapify(size-1, 0);
	return e;
}

void Heap::insert(Employee e) {
	if (size == capacity) {
		increaseCapacity();
	}
	size++;
	arr[size - 1] = e;
	arr[size - 1].performanceScore = numeric_limits<int>::min();
	increaseKey(size - 1, e.performanceScore, e.totalCalls);
}


int Heap::fillArrayFromFile(string fileName) {
	ifstream inFile;
	inFile.open(fileName);

	if (inFile.is_open() == false) {
		cout << "File could not opened" << endl;
		return -1;
	}

	string employeeId;
	string totalCalls;
	string positiveFb;
	string negativeFb;

	while (inFile.eof() == false) {

		if (size == capacity) {
			increaseCapacity();		// will not be executed
		}
		
		getline(inFile, employeeId, ',');
		getline(inFile, totalCalls, ',');
		getline(inFile, positiveFb, ',');
		inFile >> negativeFb;

		if (inFile.eof())
			break;

		arr[size].id = stoi(employeeId.c_str());
		arr[size].totalCalls = stoi(totalCalls.c_str());
		int performanceScore = stoi(totalCalls.c_str()) * 2 + stoi(positiveFb.c_str()) - stoi(negativeFb.c_str());
		arr[size].performanceScore = performanceScore;

		size++;
	}
	inFile.close();
	return arr[size - 1].id;	// relying on the text file being ascending ordered id
}

void Heap::displayEndOfDayResults(int day) {
	cout << "AFTER DAY-" << day << endl;
	
	heapSort();		// By performance
	
	List<int> minPerfList;
	int minPerformance = arr[0].performanceScore;
	minPerfList.insert(arr[0].id);
	int i = 1;
	while (minPerformance == arr[i].performanceScore) {
		minPerfList.insert(arr[i].id);
		i++;
	}

	List<int> maxPerfList;
	int maxPerformance = arr[size - 1].performanceScore;
	maxPerfList.insert(arr[size - 1].id);
	i = size - 2;
	while (maxPerformance == arr[i].performanceScore) {
		maxPerfList.insert(arr[i].id);
		i--;
	}

	cout << "BEST PERFORMANCE : ";
	maxPerfList.show();
	cout << "WORST PERFORMANCE : ";
	minPerfList.show();


	heapSortByTotalCalls();

	List<int> minCallsList;
	int minCalls = arr[0].totalCalls;
	minCallsList.insert(arr[0].id);
	i = 1;
	while (minCalls == arr[i].totalCalls) {
		minCallsList.insert(arr[i].id);
		i++;
	}

	List<int> maxCallsList;
	int maxCalls = arr[size - 1].totalCalls;
	maxCallsList.insert(arr[size - 1].id);
	i = size - 2;
	while (maxCalls == arr[i].totalCalls) {
		maxCallsList.insert(arr[i].id);
		i--;
	}

	cout << "MAXIMUM CALLS : ";
	maxCallsList.show();
	cout << "MINIMUM CALLS : ";
	minCallsList.show();

	cout << endl;
	
	this->buildMaxHeap();	// TODO: reevaluate this again (is it really necessary ? i think yes), but the output doesnt change with or without this
							// maybe its bcs the test input doesnt include all test cases. so i will keep this.

}

int main() {
	clock_t startTime;
	clock_t endTime;
	int initialHeapCapacity = 1000;
	Heap heap(initialHeapCapacity);

	string fileName = "day1.csv";
	int maxIdRead = heap.fillArrayFromFile(fileName);		// confirm this(maxIdRead) with the assistant teacher
	heap.buildMaxHeap();
	heap.displayEndOfDayResults(1);

	
	for (int i = 2; i <= 10; i++) {
		string fileName = "day" + to_string(i) +".csv";
		
		ifstream inFile;
		inFile.open(fileName);

		if (inFile.is_open() == false) {
			cout << "File could not opened" << endl;
			return 1;
		}

		string employeeId;
		string totalCalls;
		string positiveFb;
		string negativeFb;

		Employee e;
		while (inFile.eof() == false) {
			getline(inFile, employeeId, ',');
			getline(inFile, totalCalls, ',');
			getline(inFile, positiveFb, ',');
			inFile >> negativeFb;

			if (inFile.eof())
				break;
			
			e.id = stoi(employeeId.c_str());
			e.totalCalls = stoi(totalCalls.c_str());
			int performanceScore = stoi(totalCalls.c_str()) * 2 + stoi(positiveFb.c_str()) - stoi(negativeFb.c_str());
			e.performanceScore = performanceScore;
			
			int index = heap.findEmployeeIndexById(e.id);
			
			if (index == -1 || maxIdRead < stoi(employeeId.c_str())) {		// means it is a new employee
				
				heap.insert(e);		// this insertion updates the heap
			}
			else {
				Employee emp = heap.getEmployeebyIndex(index);
				int newPerfScore = e.performanceScore + emp.performanceScore;
				int newTotalCalls = e.totalCalls + emp.totalCalls;
				heap.increaseKey(index, newPerfScore, newTotalCalls);
			}
		}
		if (maxIdRead < e.id) {		// means it is a new employee
			maxIdRead = e.id;
		}
		inFile.close();
		
		heap.displayEndOfDayResults(i);		// before this function the array should be max heapified
	}
	
	/*
	*	C part: sort numbers.csv in 10 steps
	*/

	fileName = "numbers.csv";
	
	Heap heap2;		
	Employee record;	// using Employee object as a container to number (since my implemetation of Heap was according to Employees)

	ifstream inFile;
	inFile.open(fileName);
	if (inFile.is_open() == false) {
		cout << "File could not opened" << endl;
		return EXIT_FAILURE;
	}

	int counter = 0;
	int number;

	while (inFile.eof() == false) {
		
		inFile >> number;

		if (inFile.eof())
			break;

		record.id = counter++;
		record.performanceScore = number;

		heap2.insert(record);
	}
	inFile.close();

	int* records = new int[2000000];
	Employee e;
	for (int step = 0; step < 10; step++) {

		cout << "Step - " << step + 1 << endl;
		cout << "Height of the heap: " << heap2.getHeight() << endl;
		
		startTime = clock();
		for (int i = 0; i < 200000; i++) {
			records[200000 * step + i] = heap2.extractMax().performanceScore;
		}
		endTime = clock();

		cout << "Execution Time: " << (double)(endTime - startTime) / CLOCKS_PER_SEC * 1000 << " microseconds" << endl;
		cout << endl;
	}

	ofstream outputFile;
	outputFile.open("sorted.csv");

	if (outputFile.is_open() == false) {
		cout << "Output file could not created" << endl;
		return EXIT_FAILURE;
	}

	for (int i = 0; i < 2000000; i++) {
		outputFile << records[i] << endl;
	}

	outputFile.close();
	delete[] records;

	//system("pause"); // TODO: remove this line
	return 0;
}
