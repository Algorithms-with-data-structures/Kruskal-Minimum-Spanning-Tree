#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
using namespace std;

struct povezava {
	int p, q, cena;
};

struct Graf {
	int stevilo_vozlisc = 0;
	vector <povezava*> P;
};

void menu() {
	cout << "Kruskalov algoritem - izbira:" << endl;
	cout << "1) Preberi graf iz datoteke" << endl;
	cout << "2) Generiraj nakljucen graf" << endl;
	cout << "3) Pozeni" << endl;
	cout << "4) Izpis sprejetih povezav" << endl;
	cout << "5) Konec" << endl;
	cout << endl;
	cout << "Izbira: ";
}

void branje(Graf &G) {
	int stevilo_vozlisc;
	int stevilo_povezav;

	ifstream f("graf.txt");
	f >> stevilo_vozlisc;
	f >> stevilo_povezav;

	G.stevilo_vozlisc = stevilo_vozlisc;

	int vozlisceP1;
	int vozlisceP2;
	int cena;

	for (string line; getline(f, line);) {
		f >> vozlisceP1 >> vozlisceP2 >> cena;
		povezava* p1 = new povezava();
		p1->p = vozlisceP1;
		p1->q = vozlisceP2;
		p1->cena = cena;
		G.P.push_back(p1);
	}
	cout << endl;
}

void generate(Graf &G, int st_voz) {
	G.stevilo_vozlisc = st_voz;

	int j = 0;
	for (int i = 0; i < (st_voz*(st_voz - 1) / 2); i++) {			//max število povezav neusmerjenega grafa: n(n-1)/2
		povezava* p1 = new povezava();
		p1->p = (rand() % st_voz + 1);
		p1->q = (rand() % st_voz + 1);
		while (j < G.P.size()) {
			if (p1->p == G.P[j]->q && p1->q == G.P[j]->p) {			//zagotovitev, da je graf neusmerjen (če imamo povezavo od a do b, ni povezave od b do a)
				p1->p = (rand() % st_voz + 1);
				p1->q = (rand() % st_voz + 1);
			}
			else if (p1->p == G.P[j]->p && p1->q == G.P[j]->q) {	//zagotovitev, da se povezave ne ponavljajo
				p1->p = (rand() % st_voz + 1);
				p1->q = (rand() % st_voz + 1);
			}
			j++;
		}
		p1->cena = (rand() % st_voz + 1);
		G.P.push_back(p1);
	}
	cout << endl;
}

int deli(vector <povezava*>& P, int& dno, int& vrh) {
	int fixed = P[dno]->cena;
	int l = dno;
	int d = vrh;

	while (l < d) {
		while (P[l]->cena <= fixed && l < vrh)			//quick sort glede na ceno
			l++;
		while (P[d]->cena >= fixed && d > dno)
			d--;

		if (l < d)
			std::swap(P[l], P[d]);
	}

	std::swap(P[dno], P[d]);
	return d;
}

void QuickSort(vector <povezava*>& P, int dno, int vrh) {
	int j;
	if (dno < vrh) {
		j = deli(P, dno, vrh);
		QuickSort(P, dno, j - 1);
		QuickSort(P, j + 1, vrh);
	}
}

int max_plus1(vector <int> S) {
	int max = 0;
	for (int i = 0; i < S.size(); i++) {
		if (max < S[i])
			max = S[i];
	}
	return max + 1;
}

vector<povezava*> kruskal(Graf &G) {
	QuickSort(G.P, 0, G.P.size() - 1);
	vector <povezava*> R;							//rešitev (sprejete povezave)

	vector <int> S (G.stevilo_vozlisc + 1, 0);			//"mnozice povezav"
	int max;

	auto start = std::chrono::steady_clock::now();
	for (int i = 0; i < G.P.size(); i++) {						//4 scenariji
		if (S[G.P[i]->p] == 0 && S[G.P[i]->q] == 0) {
			R.push_back(G.P[i]);
			max = max_plus1(S);
			S[G.P[i]->p] = max;
			S[G.P[i]->q] = max;
		}
		else if (S[G.P[i]->p] == S[G.P[i]->q])					//če sta p in q v enaki množici - cikel, ignoriramo
			continue;										 
		else if (S[G.P[i]->p] != S[G.P[i]->q]) {				//če sta p in q v različnih množicah ... 
			R.push_back(G.P[i]);
			if (S[G.P[i]->p] != 0) {
				for (int j = 0; j <= S.size(); j++) {				//... množici združimo - vse vrednosti v S iz dveh množic (oz. z dvema različnima vrednostima v tabeli) nastavimo na enako vrednost
					if (S[G.P[i]->q] == S[G.P[j]->q])
						S[G.P[j]->q] = S[G.P[i]->p];
				}
			}
		}
		else if (S[G.P[i]->p] != 0 && S[G.P[i]->q] == 0 || S[G.P[i]->p] == 0 && S[G.P[i]->q] != 0) {	//p ni 0 in q je 0, ali obratno
			R.push_back(G.P[i]);
			if (S[G.P[i]->p] == 0)
				S[G.P[i]->p] = S[G.P[i]->q];
			else											//p damo v isto množico kot q ali obratno
				S[G.P[i]->q] = S[G.P[i]->p];
		}

		if (R.size() == (G.stevilo_vozlisc - 1)) {
			auto end = std::chrono::steady_clock::now();
			cout << "Stevilo vhodnih vozlisc: " << G.stevilo_vozlisc << endl;
			cout << "Stevilo vhodnih povezav: " << G.P.size() << endl;
			cout << "Stevilo sprejetih povezav: " << R.size() << endl;
			cout << "Cas trajanja kruskalovega algoritma " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "μs." << endl;
			cout << endl;
			return R;
		}
	}

	cout << "Ni resitve!" << endl; 
}

void print(Graf G) {
	for (int i = 0; i < G.P.size(); i++) {
		cout << "p: " << G.P[i]->p << " q: " << G.P[i]->q << " cena: " << G.P[i]->cena << endl;
	}
	cout << endl;
}

void print_sprejete(vector <povezava*> R) {
	for (int i = 0; i < R.size(); i++) {
		cout << "p: " << R[i]->p << " q: " << R[i]->q << " cena: " << R[i]->cena << endl;
	}
	cout << endl;
}


int main() {
	int selection;
	bool running = true;

	Graf G;
	Graf G1;
	Graf G2;
	int st_voz;

	vector <povezava*> R;

	do {
		menu();
		cin >> selection;
		switch (selection) {
		case 1:
			branje(G1);
			print(G1);
			G = G1;
			break;
		case 2:
			cout << "Stevilo vozlisc grafa: ";
			cin >> st_voz;
			generate(G2, st_voz);
			//print(G2);
			G = G2;
			break;
		case 3:
			R = kruskal(G);
			break;
		case 4:
			print_sprejete(R);
			break;
		case 5:
			running = false;
			break;
		default:
			cout << "ERROR";
			break;
		}
	} while (running);

	return 0;
}

