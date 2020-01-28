#include<iostream>
#include <vector>
#include <string>
using namespace std;
const int N = 100;


struct Vershina;
struct Edge;

class Link
{
public:
	Vershina *versh;
	Edge *edge;

	Link() {
		versh = nullptr;
		edge = nullptr;
	}
	Link(Vershina* ver) {
		versh = ver;
		edge = nullptr;
	}
	Link(Edge *ed) {
		versh = nullptr;
		edge = ed;
	}
	Link& operator= (const Link& link) {
		versh = link.versh;
		edge = link.edge;

		return *this;
	}
	void markVert();
	bool isVertex();
	int getVert();
	Link getNext();
	bool isNull();
	bool isEdge();
	void passEdge();
	Link getEnd();
	bool isUnmarkedVertex();
	void setNext(Link temp);
	void passVert();
};

struct Edge {
	Edge *nextEdge;
	Vershina *versh;
	bool mark;
	Vershina *begin;
	Vershina *end;
	Link link;

	Edge() {
		
		begin = nullptr;
		end = nullptr;
		nextEdge = nullptr;
		versh = nullptr;
	}
	Edge(Vershina * versh) {
		nextEdge = nullptr;
		this->versh = versh;
	}


};
struct Vershina
{
	int num;
	enum Mark { UNMARKED, MARKED, PASSED };
	Mark mark;
	int value;
	Vershina *next;
	Edge *node;
	Link link;

	Vershina(int v) {
		num = 0;
		value = v;
		next = nullptr;
		node = nullptr;
		mark = Mark::UNMARKED;
	}
	Vershina() {
		next = nullptr;
		node = nullptr;
		mark = Mark::UNMARKED;
	}
	void puss() {
		mark = MARKED;
	}
	void linkInit() {
		link = Link(node);
		Edge* temp = node;
		while (temp != nullptr) {
			temp->link = Link(temp->nextEdge);
			temp = temp->nextEdge;
		}
	}
	void removeEdge(int index);

};
void Vershina::removeEdge(int index) {

	Edge* tmpE = this->node;
	Edge* temp;
	if (tmpE == nullptr)
		return;
	if (tmpE->end->value == index) {
		node = tmpE->nextEdge;
		delete tmpE;
		return;
	}
	else {
		while (tmpE->nextEdge != nullptr) {
			if (tmpE->nextEdge->end->value == index) {
				temp = tmpE->nextEdge;
				tmpE->nextEdge = tmpE->nextEdge->nextEdge;
				temp->nextEdge = nullptr;
				delete temp;
				return;
			}
			tmpE = tmpE->nextEdge;
		}
	}

}

Link Link::getNext() {
	if (versh != nullptr) {
		return versh->link;
	}
	else if (edge != nullptr) {
		return edge->link;
	}
	return Link();
}
void Link::markVert() {
	if (isVertex()) {
		versh->mark = Vershina::Mark::MARKED;
	}
}

bool Link::isVertex() {
	return (versh != nullptr && edge == nullptr);
}

int Link::getVert() {
	if (isVertex()) {
		return versh->value;
	}
	return -1;
}
bool Link::isNull() {
	return (versh == nullptr && edge == nullptr);
}
bool Link::isEdge() {
	return (edge != nullptr && versh == nullptr);
}
void Link::passEdge() {
	if (isEdge()) {
		edge->mark = true;
	}
}
Link Link::getEnd() {
	if (isEdge()) {
		return Link(edge->end);
	}
	return Link();
}
bool Link::isUnmarkedVertex() {
	return versh->mark == Vershina::Mark::UNMARKED;
}
void Link::setNext(Link temp) {
	if (versh != nullptr) {
		versh->link = temp;
	}
	else if (edge != nullptr) {
		edge->link = temp;
	}
}
void Link::passVert() {
	if (isVertex()) {
		versh->mark = Vershina::Mark::PASSED;
	}
}
class Graf {
private:
	int count;
	Vershina *Head;

	void LinkInit() {
		Vershina *temp = Head;
		while (temp != nullptr) {
			temp->linkInit();
			temp = temp->next;
		}
	}

	void unmark() {
		Vershina* tempV = Head;
		while (tempV != nullptr) {
			tempV->mark = Vershina::UNMARKED;
			Edge* tempE = tempV->node;
			while (tempE != nullptr) {
				tempE->mark = false;
				tempE = tempE->nextEdge;
			}
			tempV = tempV->next;
		}
	}

	Vershina *searchVersh(int p) {
		Vershina *versh = Head;
		while (versh) {
			if (versh->value == p) return versh;// нашел
			versh = versh->next;
		}
		return  nullptr;
	}

	bool searchVertexh(int newAdjacentTop) {// функция поиска вершины.
		Vershina *versh = Head;
		while (versh) {
			if (versh->value == newAdjacentTop) return true;// нашел
			versh = versh->next;
		}
		return false;
	}

	bool searchEdge(int topIncoming, int topEntry) { // функция поиска ребра
		if (searchVertexh(topIncoming) && searchVertexh(topEntry)) {
			Vershina *v1 = Head;
			while (v1->value != topIncoming) v1 = v1->next;
			Vershina *v2 = Head;
			while (v2->value != topEntry) v2 = v2->next;
			Edge *node = v1->node;
			while (node) {
				if (node->versh == v2) return true;
				node = node->nextEdge;
			}
			return false;
		}return false;
	}
	Edge* SearchEdge(int topIncoming, int topEntry) { // функция поиска ребра
		if (searchVertexh(topIncoming) && searchVertexh(topEntry)) {
			Vershina *v1 = Head;
			while (v1->value != topIncoming) v1 = v1->next;
			Vershina *v2 = Head;
			while (v2->value != topEntry) v2 = v2->next;
			Edge *node = v1->node;
			while (v1->node) {
				if (node->versh == v2) return node;
				node = node->nextEdge;
			}
			return nullptr;
		}return nullptr;
	}

public:
	Graf() {

		count = 0;
	};//конструктор
	~Graf() {
		while (Head) {
			Vershina *temp = Head->next;
			delete Head;
			Head = temp;
		}
	}
	void addVertex(int newAdjacentTop) { // если такой вершины нет, то добавляет ее.
		if (Head != nullptr) {
			if (!searchVertexh(newAdjacentTop)) {
				Vershina *temp = Head;
				while (temp->next) temp = temp->next;
				temp->next = new Vershina(newAdjacentTop);
				count++;
			}
		}
		else {
			Head = new Vershina(newAdjacentTop);
			count++;
		}
	}
	void addEdge(int topIncoming, int topEntry) { // Добавление ребра из topIncoming в  topEntry. Если вершины есть, они не одинаковые, и такого ребра нет, то добавляет ребро.
		if (searchVertexh(topIncoming) != true || searchVertexh(topEntry) != true) return;
		if (topEntry == topIncoming) return;
		if (!searchEdge(topIncoming, topEntry)) {
			Vershina *v1 = Head;
			while (v1->value != topIncoming) v1 = v1->next;
			Vershina *v2 = Head;
			while (v2->value != topEntry) v2 = v2->next;
			Edge *node = v1->node;
			if (node) {
				while (node) {
					if (node->nextEdge == NULL) {
						node->nextEdge = new Edge;
						node->nextEdge->versh = v2;
						node->nextEdge->begin = v1;
						node->nextEdge->end = v2;
						return;
					}
					node = node->nextEdge;
				}
			}
			else {
				v1->node = new Edge;
				v1->node->versh = v2;
				v1->node->begin = v1;
				v1->node->end = v2;

			}
		}
	}

	
	void BFS(int index) {
		unmark();
		LinkInit();
		Vershina *P = searchVersh(index);
		Link F = Link(P);
		Link V = F;
		F.markVert();
		cout << F.getVert() << " ";
		while (!F.getNext().isNull()) {
			F = F.getNext();
		}
		while (!V.isNull()) {
			Link E = V;
			while (!E.getNext().isNull() && E.getNext().isEdge()) {
				E = E.getNext();
				E.passEdge();
				Link Q = E.getEnd();
				if (Q.isUnmarkedVertex()) {
					F.setNext(Q);
					F = Q;
					Q.markVert();
					cout << Q.getVert() << " ";
					while (!F.getNext().isNull()) {
						F = F.getNext();
					}
				}
			}
			V.passVert();
			V = E.getNext();
			E.setNext(Link());
		}
		cout << endl;
	}
	void deleteEdge(int begin, int end) {
		if (end == begin) return;
		if (!searchVertexh(end)) return;
		if (!searchVertexh(begin)) return;
		if (!searchEdge(begin, end))return;

		Vershina* tmpV = searchVersh(begin);
		if (tmpV != nullptr) {
			tmpV->removeEdge(end);
		}
		return ;

		//Edge *edge = SearchEdge(begin, end);
		//Vershina *v = searchVersh(begin);
		//Edge* tmpE = v->node;
		//Edge* temp;
		//if (tmpE == nullptr)
		//	return;
		//if (tmpE->end == edge->end) {
		//	Head->node = tmpE->nextEdge;
		//	delete tmpE;
		//	return;
		//}
		//else {
		//	while (tmpE->nextEdge != nullptr) {
		//		if (tmpE->nextEdge->end == edge->end) {
		//			temp = tmpE->nextEdge;
		//			tmpE->nextEdge = tmpE->nextEdge->nextEdge;
		//			temp->nextEdge = nullptr;
		//			delete temp;
		//			return;
		//		}
		//		tmpE = tmpE->nextEdge;
		//	}
		//}
	}

	void deleteVertex(int index) {
		if (!searchVersh(index))
			return;
		
		Vershina * versh = searchVersh(index);
		Vershina *t = Head;
		while (t) {
				deleteEdge(t->value, index);
				deleteEdge(index, t->value);
			t = t->next;
	};

		Edge *ed = versh->node;
		Edge *temp1;
	/*	while (ed) {
			temp1 = ed;
			ed = ed->nextEdge;
			delete temp1;
		}*/
		Vershina *tempV = Head;
		Vershina* temp;
		if (tempV == nullptr)
			return;
		if (tempV->value == index) {
			Head = tempV->next;
			delete tempV;
			count--;
			return;
		}
		else {
			while (tempV->next != nullptr) {
				if (tempV->next->value == index) {
					temp = tempV->next;
					tempV->next = tempV->next->next;
					delete temp;
					count--;
					return;
				}
				tempV = tempV->next;
			}
		}
		return;
	}

	string strongConnectedComponents() { //светлана николаевна, не читайте этот код, там все ООООООЧЕНЬ ПЛОХО
		//инцилизация 
		if (count == 0) return "";
		int** arr = new int*[count];
		for (int i = 0; i < count; i++) {
			arr[i] = new int[count];
		}
		for (int i = 0; i < count; i++) {
			for (int j = 0; j < count; j++) {
				arr[i][j] = 0;
			}
		}
		//матрциа смежности

		Vershina *versh = Head;
		int a = 0;
		while (versh) {
			versh->num = a++;
			versh = versh->next;
		}
		versh = Head;
		Edge *ed = versh->node;
		while (versh) {
			ed = versh->node;
			while (ed) {
				arr[versh->num][ed->versh->num] = 1;
				ed = ed->nextEdge;
			}
			versh = versh->next;
		}

		//матрица смежности 

	/*	g.addEdge(1, 2);
		g.addEdge(1, 3);
		g.addEdge(1, 4);
		g.addEdge(4, 3);*/


		//for (int i = 0; i < count; i++) { //матрциа смежности
		//	for (int j = 0; j < count; j++) {
		//		arr[i][j] = arrSmejj[i][j];
		//	}
		//}
		int** arr2 = new int*[count];
		for (int i = 0; i < count; i++) {
			arr2[i] = new int[count];
		}
		////////инцилизация матриц
	
		//инцилизация матрциы достижимости
		for (int k = 0; k < count; k++)
			for (int i = 0; i < count; i++) {
				if (arr[i][k] == 1) {
					for (int j = 0; j < count; j++)
						arr[i][j] = (arr[i][j] || arr[k][j]);
				}
			}
		//инцилизация матрицы достижимости

	//транспонирование матрицы достижимости
		for (int i = 0; i < count; i++) { //матрциа смежности
			for (int j = 0; j < count; j++) {
				arr2[i][j] = arr[i][j];
			}
		}

		int t;
		for (int i = 0; i < count; ++i)
		{
			for (int j = i; j < count; ++j)
			{
				t = arr2[i][j];
				arr2[i][j] = arr[j][i];
				arr2[j][i] = t;
			}

		}
	//транспонирование матрицы достижимости

		for (int i = 0; i < count; i++) { 
			for (int j = 0; j < count; j++) {
				arr[i][j] = arr[i][j] * arr2[i][j];
		
			}
		}
		string* str = new string[count];

		for (int i = 0; i < count; i++) {
			for (int j = 0; j < count; j++) {
				if (arr[i][j] == 1) {
					Vershina *versh = Head;
					while (versh->num!=j) {
						versh = versh->next;
					}
					str[i] += to_string(versh->value)+ " ";
				}
			}
		}
		string* str2 = new string[count];
		int c = 0;
		for (int i = 0; i < count; i++) { //матрциа смежности
			for (int j = 0; j < count; j++) {
				if(i!=j){
					if (str[i] == str[j]) {
						str[j] = ""; str2[i] = str[i]; c++;
					}
				}
			}
		}
		delete[] str;
		string str1;
		c = 0;
		for (int i = 0; i < count; i++) { //матрциа смежности
			if (str2[i] != "" && str2[i] != " ") { str1 += "{ " + str2[i] + "} "; }
		}
		return str1;
	}
	void printGraf() {
		Vershina *versh = Head;
		Edge *ed;
		while(versh){
			ed = versh->node;
			cout << versh->value << ": ";
			while (ed) {
				cout << " -> " << ed->versh->value;
				ed = ed->nextEdge;
		}
			cout << endl;
			versh = versh->next;
		}
	}

};

int main() {

	Graf g;
	//g.addVertex(1); //добавление вершины. Повторяющихся вершин быть не может
	//g.addVertex(2); //добавление вершины. Повторяющихся вершин быть не может
	//g.addEdge(1, 2); //добавление ребра. Из 1 в 2.
	//g.printGraf(); //печатанье графа в виде списка смежности
	//g.deleteEdge(1, 2);//удаление ребря (1,2)
	//g.deleteVertex(1);// удаление вершины 1
	//g.BFS(2);//поиск в ширину
	//cout << g.strongConnectedComponents() << endl;//поиск сильных компонент(не читайте ту функцию, там треш, мне за нее очень стыдно, но я очень торопился)

	//
	for (int i = 1; i <= 9; i++)
	g.addVertex(i);

	g.printGraf();
	cout << endl;

	g.addEdge(1, 2);
	g.addEdge(2, 3);
	g.addEdge(3, 1);
	g.addEdge(4, 5);
	g.addEdge(5, 6);
	g.addEdge(6, 4);
	g.addEdge(7, 8);
	g.addEdge(8, 9);
	g.addEdge(9, 7);

	g.printGraf();

	cout << g.strongConnectedComponents() << endl;
	cout << endl;

	//g.deleteVertex(1);

	//g.printGraf();
	//cout << endl;

	//g.addVertex(1);

	//g.printGraf();
	//cout << endl;

	//g.BFS(2);

	//cout << endl;
	//g.printGraf();

	//cout << endl;
	//cout << g.strongConnectedComponents() << endl;

	//g.deleteVertex(7);

	//g.printGraf();
	//cout << endl;

	//g.BFS(2);
	system("pause");
}