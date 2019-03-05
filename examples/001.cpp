#include <iostream>
#include <vector>
#include <mutex>
#include <thread>

// STATIC_INIT_ORDER_FIASCO
struct helper_struct{
	int var;
	helper_struct(int n) : var(n) {}
	int square(){
		return var*var;
	}
};
struct static_init_order_fiasco{
	static helper_struct hs;
	static int square;
};

int square{ static_init_order_fiasco::hs.square() };
helper_struct static_init_order_fiasco::hs{10};


// EMPTY_VECTOR_ACCESS
int empty_vector_access( unsigned int n ){
	const std::vector<int> V;
	return V[0];
}

// LOCK CONSISTENCY VIOLATION
struct thread_safe_array{
private:
	std::vector<int> data;
	int x;
	std::mutex lock;
	int n;
public:
	thread_safe_array( int n ) : data(n), n(n) {}
	int &read( int i ){
		return data[i];
	}
	int &read(){
		return x;
	}
	void write( int i, int val ){
		std::lock_guard<std::mutex> lk(lock);
		data[i] = val;
	}
	void write( int val ){
		x = val;
	}
	int size(){
		return n;
	}
};

void thread_safe_array_reader( thread_safe_array &tsa ){
	for( int i=0; i<tsa.size(); ++i )
		tsa.read(i);
}

void thread_safe_array_writer( thread_safe_array &tsa ){
	for( int i=0; i<tsa.size(); ++i )
		tsa.write(i, i*i);
}

void thread_safe_reader( thread_safe_array &tsa ){
	for( int i=0; i<tsa.size(); ++i )
		std::cout << tsa.read() << ' ';
	std::cout << '\n';
}

void thread_safe_writer( thread_safe_array &tsa ){
	for( int i=0; i<tsa.size(); ++i )
		tsa.write(i*i);
}

void lock_consistency_violation( int n ){
	thread_safe_array tsa(n);
	std::thread t1( thread_safe_reader, std::ref(tsa) ),
				t2( thread_safe_writer, std::ref(tsa) );
	t1.join();
	t2.join();
}

int main(){
	std::vector<char> V;
	int a = V[0];
	lock_consistency_violation( a );
	auto W{ empty_vector_access( a ) };
}