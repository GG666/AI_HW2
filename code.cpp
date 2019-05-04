#include<bits/stdc++.h>
#define num_trees 50  //decide the number of trees that we want to build
#define file_size 200 //depend on the testdata of file
using namespace std;

vector<vector<double>> v(file_size);			//Store all the data
vector<vector<double>> sorted_by_x(file_size);  //Store all the data that sorted by x
vector<vector<double>> sorted_by_y(file_size);  //Store all the data that sorted by y

//Basic object
class obj{
	public:
		obj(double x,double y,int z){
			this->x=x;
			this->y=y;
			this->z=z;
		}
		double x;
		double y;
		int z;
};

//The linked-list of binary tree node
class node{
	public:
		node(){
			L=NULL;
			R=NULL;
			V.clear();
		}
		vector<int> V;
		bool classificate_by_xy;
		double threshold;
		node* L;
		node* R;
};

//To classification a data by the tree
int tree_test(vector<node*> &v,obj &o){
	int i=0,one=0,two=0;
	while(i<v.size()){

			//At right node
			if(o.x>v[i]->threshold){
				for(int j=0;j<(v[i]->R->V).size();j++)
					((v[i]->R->V)[j]==1)? one++:two++;
			}
		i++;
	}
	return one>=two? 1:2;
}

//Let tree to vote(training)
int vote(vector<vector<obj*>> &forest,obj* o){
	int one=0,two=0,tt;
	for(int i=0;i<num_trees;i++){
		tt=tree_test(forest[i], &o);
		tt==1? one++:two++;
	}
	return one>=two? 1:2;
}

//To calculate the Gini index
double Gini(vector<vector<double>> &v,int begin,int end){
	int one=0,two=0,count=0;
	for(int i=begin;i<end;i++){
		v[i][2]==1? one++:two++;
		count++;
	}
	return 1-(one/count)*(one/count)-(two/count)*(two/count);
}

//To make sort function sort by x
bool sortx(const vector<double>& v1,const vector<double>& v2){
	return v1[0]<v2[0]; 
}

//To make sort function sort by y
bool sorty(const vector<double>& v1,const vector<double>& v2){
	return v1[1]<v2[1]; 
}

//to claculate the location of min-threshold
int min_thre_loc(vector<vector<double>> &vx,vector<vector<double>> &vy,bool b,double &min){
	double min=INT_MAX,tt;
	int size=v.size(),loc;											//Find threshold loc
	if(b){														//sortx
		for(int i=1;i<size;i++){
			tt=Gini(vx,0,i)*i/(double)size+Gini(vx,i+1,size)*(size-i)/(double)size;
			if(tt<min){
				min=tt;
				loc=i;
			}
		}
	}
	else{														//sorty
		for(int i=0;i<size-1;i++){
			tt=Gini(vy,0,i)*i/size+Gini(vy,i+1,size)*(size-i)/size;
			if(tt<min){
				min=tt;
				loc=i;
			}
		}
	}
	return thre;
}

//Build a tree
void build_tree(int layer,node* &cur){
	bool xy;
	int thre,tt,temp;
	double minx,miny,min;

	//Find the threshold location
	tt=min_thre_loc(sorted_by_x,1,minx);
	temp=min_thre_loc(sorted_by_y,0,miny);
	if(minx<miny){
		xy=1;		//x
		thre=tt;
	}
	else{
		xy=0;		//y
		thre=temp;
	}

	//Create new node
	node* l=new node();
	node* r=new node();

	//To split the data
	for(int i=0;i<=thre;i++){
		if(xy){
			obj* o=new obj(sorted_by_x[i][0],sorted_by_x[i][1],sorted_by_x[i][2]);
			l->V.push_back(sorted_by_x[i][2]);
			l->classificate_by_xy=1;
		}
		else{
			obj* o=new obj(sorted_by_y[i][0],sorted_by_y[i][1],sorted_by_y[i][2]);
			l->V.push_back(sorted_by_y[i][2]);
			l->classificate_by_xy=0;
		}
		l->threshold=thre;
	}
	cur->L=l;
	for(int i=thre+1;i<sorted_by_x.size();i++){
		if(xy){
			obj* o=new obj(sorted_by_x[i][0],sorted_by_x[i][1],sorted_by_x[i][2]);
			r->V.push_back(sorted_by_x[i][2]);
			r->classificate_by_xy=1;
		}
		else{
			obj* o=new obj(sorted_by_y[i][0],sorted_by_y[i][1],sorted_by_y[i][2]);
			r->V.push_back(sorted_by_y[i][2]);
			r->classificate_by_xy=0;
		}
		r->threshold=thre;
	}
	cur->R=r;
	if(layer==1){
		layer++;
		build_tree(layer,cur->R);
	}
}

int main(){
	//speed up
	cin.tie(0);ios_base::sync_with_stdio(0);

	//To read the file and store the data in a 2D vector "v"
	int count=0;
	fstream f;
	f.open("cross200.txt",ios::in);
	double a;
	string s;
	stringstream ss;
	v.resize(200);
	bag.resize(200,-1);
	while(getline(f,s)){
		ss<<s;
		while(ss>>a)
			v[count].push_back(a);	
	}

	//make 2D vector sorted_by_x and sorted_by_y sorted
	sorted_by_x=v;
	sorted_by_y=v;
	sort(sorted_by_x.begin(),sorted_by_x.end(),sortx);
	sort(sorted_by_y.begin(),sorted_by_y.end(),sorty);

	//To use rand function
	srand(time(0));

	//A container that help us to pick datas for building a tree
	vector<bool> bag(num_trees);

	//Array of tree
	node* tree[num_samples];
	
	//A container which contains all the trees and the nodes that belong to it
	vector<vector<node*>> forest(num_trees);

	//Decide the nodes that a tree contains (it may drop some datas)
	int num_samples=v.size()/num_trees;

	//build the all the validation tree
	for(int i=0;i<num_trees-1;i++){
		int counter=0,ran;

		//Choose datas for a tree
		while(counter<num_samples){

			//To pick datas randomly
			ran=rand()%bag.size();
			
			// Find a new node of a tree
			if(!bag[ran]){
				obj* o=new obj(v[ran][0],v[ran][1],v[ran][2]);
				bag[ran]=1;
				forest[i].push_back(o);
				counter++;
			}
		}
		//Building a tree
		build_tree(1,forest[i][0]);
	}

	//Numerate and counter
	int correct=0,coco=0;

	//calculate the correction classification rate
	for(int i=0;i<v.size();i++){

		//rest of data
		if(!bag[i]){
			obj* o=new obj(v[i][0],v[i][1],v[i][2]);
			if(vote(forest[i],o)==v[i][2])
				correct++;
			coco++;
		}
	}

	//Print the result
	cout<<"accuracy:"<<correct/coco<<endl;
}