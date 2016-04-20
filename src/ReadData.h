#include <string>
using std::string;

struct vector{
	float x;
	float y;
	bool trueData;
};

class ReadData{
	public:
		ReadData();
		virtual ~ReadData();
		
		void readFromFile(string fileName, string groupName, 
						  string compXName, string compYName);
		
		vector getVector(int x, int y);
		
	private:
		float dataXComp;
		float dataYComp;
		
};