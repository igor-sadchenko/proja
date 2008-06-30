
//use this in the begining of the .CPP
#define INIT_SINGLETON2(T) T T::m_instance;
#define INIT_SINGLETON(T) T* T::m_instance;

//use this in the class devtlaration
#define SINGLETON(T) private: static T m_instance;	\
	T(){}								\
	public: static T& getInstance(void)		\
	{return m_instance;}				\
	static T* getInstancePtr(void)			\
	{return &m_instance;}

template<typename T>
class Singleton
{
public:

	static T* m_instance;

	Singleton()
	{
		m_instance = (T*)this;
	}
	static T& getInstance()
	{
		return *m_instance;
	}
	static T* getInstancePtr()
	{
		return m_instance;
	}
};