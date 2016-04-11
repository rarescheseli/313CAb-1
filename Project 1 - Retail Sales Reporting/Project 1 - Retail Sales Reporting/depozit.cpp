#include "Classes.h"

Depozit_Magazin::Depozit_Magazin() {
	
}

Depozit_Magazin::Depozit_Magazin(ResizableArray<Produs*> produse, ResizableArray<int> nr_produse) {
	this->produse = produse;
	this->nr_produse = nr_produse;
};

Depozit_Magazin::Depozit_Magazin(const Depozit_Magazin& other)
{
	this->produse = other.produse;
	this->nr_produse = other.nr_produse;
}

Depozit_Magazin::~Depozit_Magazin() {

}


int Depozit_Magazin::findProdus(int id)
{
	int i, n = produse.size();
	for (i = 0; i < n; ++i)
	{
		if (produse[i]->getId() == id) return i;
	}
	return -1;
}

int Depozit_Magazin::getNProdus(int id)
{
	int pos = findProdus(id);
	if (pos == -1) return 0;
	return nr_produse[pos];
}

void Depozit_Magazin::addProdus(Produs* produs, int cantitate)
{
	produse.push_back(produs);
	nr_produse.push_back(cantitate);
}

void Depozit_Magazin::addProdus(int pos, int cantitate)
{
	nr_produse[pos] += cantitate;
}

Depozit_Magazin& Depozit_Magazin::operator= (const Depozit_Magazin& other)
{
	this->produse = other.produse;
	this->nr_produse = other.nr_produse;
	return *this;
}