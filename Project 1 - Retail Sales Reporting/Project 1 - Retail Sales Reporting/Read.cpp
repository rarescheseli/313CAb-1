#include "Classes.h"

void Read::BonuriRead(const char* name, Hash <string, int>& hBonuri) {
	bonurif.open(name);

	string line;
	string id_bon;
	int id_produs;
	int sz;
	int i;

	// Sar peste headerele tabelului
	getline(bonurif, line, '\r');
	while (getline(bonurif, line, '\r')) {
		sz = line.size();

		id_bon = "";
		id_produs = 0;

		// Fac split caracter cu caracter la linia citita
		i = 0;
		while (line[i] != ',') {
			id_bon += line[i++];
		}

		i++;
		while (i < sz) {
			id_produs = id_produs * 10 + line[i++] - '0';
		}

		hBonuri.insert(id_bon, id_produs);
	}
}

time_t Read::ConvertTime(char *timeToConvert) {
	struct tm newTime = { 0 };
	char *dates[7];
	char *p;
	int nr = 0;

	timeToConvert[strlen(timeToConvert)] = '\0';
	p = strtok(timeToConvert, "- : ");
	while (p != NULL) {
		dates[nr++] = p;
		p = strtok(NULL, "- : ");
	}

	newTime.tm_year = atoi(dates[0]) - 1900;
	newTime.tm_mon = atoi(dates[1]) - 1;
	newTime.tm_mday = atoi(dates[2]);
	newTime.tm_hour = atoi(dates[3]);
	newTime.tm_min = atoi(dates[4]);
	newTime.tm_sec = atoi(dates[5]);

	time_t temp = mktime(&newTime);


	return temp;
}

void Read::ProduseRead(const char *fileName, Produs *produse, Categorii& cat) {

	string line = "";
	produsef.open(fileName);

	int prodId;
	string prodName;
	int prodCategory;
	int prodPrice;
	int pos, length;


	getline(produsef, line);
	while (getline(produsef, line)) {

		if (line != "") {

			prodName = "";
			prodId = 0;
			prodCategory = 0;
			prodPrice = 0;
			length = line.size();
			pos = 0;


			while (line[pos] != ',') {
				prodId = prodId * 10 + line[pos++] - '0';
			}

			++pos;
			while (line[pos] != ',') {
				prodName += line[pos++];
			}

			++pos;
			while (line[pos] != ',') {
				prodCategory = 10 * prodCategory + short(line[pos++] - '0');
			}

			++pos;
			while (pos < length) {
				prodPrice = 10 * prodPrice + short(line[pos++] - '0');
			}

			produse[prodId].AddData(prodId, prodName, cat.Get(prodCategory), prodPrice);

		}
	}
}

void Read::PaletiRead(const char* fileName, DepozitGlobal &d) {

	string line = "";
	int pos, length;
	paletif.open(fileName);

	string paletId = "";
	short prodType = 0, nItems = 0, slot = 0;

	paletif >> line;
	while (paletif >> line) {
		paletId = "";
		prodType = nItems = slot = 0;
		pos = 0;
		length = line.size();

		while (line[pos] != ',') {
			paletId += line[pos++];
		}

		++pos;
		while (line[pos] != ',') {
			prodType = 10 * prodType + short(line[pos++] - '0');
		}

		++pos;
		while (line[pos] != ',') {
			nItems = 10 * nItems + short(line[pos++] - '0');
		}

		++pos;
		while (pos < length) {
			slot = 10 * slot + short(line[pos++] - '0');
		}

		// Adaugare date la structuri

		Palet p(paletId, nItems, prodType);
		d.GetSloturi()[slot].push(p);

	}

}

void Read::MagazineRead(const char* fileName, ResizableArray<Magazin> &magazine, Produs *prod) {
	magazinef.open(fileName);

	string line;
	int id_magazin;
	string locatie;
	int i, sz;
	Magazin empty;

	// Sar peste headerele tabelului
	getline(magazinef, line);
	while (!magazinef.eof()) {
		getline(magazinef, line);
		id_magazin = 0;
		locatie = "";
		sz = line.size();
		if (line != "") {
			// Fac split caracter cu caracter la linia citita
			i = 0;
			while (line[i] != ',') {
				id_magazin = id_magazin * 10 + line[i++] - '0';
			}

			++i;
			while (i < sz) {
				locatie += line[i++];
			}

			Magazin m(locatie, prod);

			while (id_magazin > (int)magazine.size())
			{
				magazine.push_back(empty);
			}


			magazine[id_magazin-1] = m;
		}
	}
}

void Read::CategoriiRead(const char *fileName, Categorii& cat) {

	categoriif.open(fileName);

	string line;
	string categorie;
	int id_categorie;
	int i;

	// Sar peste headerele tabelului
	getline(categoriif, line);

	while (!categoriif.eof()) {

		getline(categoriif, line);
		id_categorie = 0;
		categorie = "";

		if (line != "") {

			// Fac split caracter cu caracter la linia citita
			i = 0;
			while (line[i] != ',') {
				id_categorie = id_categorie * 10 + line[i++] - '0';
			}

			i++;
			while (i < (int)line.length()) {

				categorie += line[i++];

			}

			cat.Add(id_categorie, categorie);

		}
	}


}

void Read::TranzactiiRead(const char *fileName, ResizableArray<Magazin>& magazine,
	ResizableArray < Bon < int, string, time_t > > &bonuri) {
	tranzactiif.open(fileName);

	unsigned short zile_trecute[] = { 0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };
	char timestamp[30], *aux, tmp[30];
	int id_magazin, i, n, zi;
	string id_bon, line;

	// Sar peste headerele tabelului
	getline(tranzactiif, line, '\r');
	while (!tranzactiif.eof()) {
		getline(tranzactiif, line, '\r');
		id_magazin = 0;
		id_bon = "";

		if (line != "") {
			// Fac split caracter cu caracter la linia citita
			i = 0;
			while (line[i] != ',') {
				++i;
			}

			++i; n = 0;
			while (line[i] != ',') {
				timestamp[n++] = line[i++];
			}

			++i;
			timestamp[n] = '\0';
			while (line[i] != ',') {
				id_bon += line[i++];
			}

			++i;
			while (i < (int)line.length()) {
				id_magazin = id_magazin * 10 + line[i++] - '0';
			}

			for (int k = 0; k < (int)strlen(timestamp); ++k) {
				tmp[k] = timestamp[k];
			}

			aux = strtok(tmp, "-");
			aux = strtok(NULL, "-");
			zi = zile_trecute[10 * (aux[0] - '0') + (aux[1] - '0')];
			aux = strtok(NULL, " ");
			zi += atoi(aux);

			magazine[id_magazin - 1].add_bon(id_bon, zi);
			Bon < int, string, time_t > tmp;
			tmp.idBon = id_bon;
			tmp.idMag = id_magazin;
			tmp.timestamp = ConvertTime(timestamp);
			bonuri.push_back(tmp);
		}
	}
}




Read::~Read() {

	bonurif.close();
	categoriif.close();
	magazinef.close();
	paletif.close();
	produsef.close();

}