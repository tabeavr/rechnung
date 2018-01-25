#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <string_view>
#include <iomanip>
#include <stdio.h>
#include <boost/program_options.hpp>
#include <unistd.h>
namespace po = boost::program_options;


//Ein Produkt muss einen Namen und einen Preis haben, die Anzahl ist defaultmäßig bei 1 und kann bei Bedarf geändert werden.
struct Produkt{
	public:
		Produkt();
		void writeProduktInfo(int counter);	
		std::string produktname{};
		double preis = 0.0;
		int anzahl = 0;
		double gesamtPreis = 0.0;
};
Produkt::Produkt(){
	std::cout << "Gib den Namen des Produktes ein" << std::endl;
	std::cin.sync();
	std::cin.clear();
	std::getline(std::cin, produktname);
	//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout << "Gib den Preis des Produktes ein in der Form xxxx.yy EUR" << std::endl;
	std::cin >> preis;
	std::cout << "Gib die Anzahl der verkaufte Produkte ein" << std::endl;
	std::cin >> anzahl;
	gesamtPreis = preis * (double) anzahl;
}

void Produkt::writeProduktInfo(int counter){
	std::ofstream produktinfo;
	produktinfo.open("Produkte.csv", std::ios::app);
	produktinfo << counter <<" " <<this->produktname<< " "<< this->anzahl<< " " << this->preis << " " << this->gesamtPreis << ";" << std::endl;
	produktinfo.close();
}


void produktDateivorbereiten(){
	std::ofstream produktinfo;
	produktinfo.open("Produkte.csv", std::ofstream::trunc); //überschreibt datei
	produktinfo << "Position" <<" " <<"Posten"<< " "<< "Anzahl"<< " " << "Einzelpreis" << " " << "Gesamtpreis" << std::endl;
	produktinfo.close();
	std::cout << "es kann losgehen" << std::endl;
}

int main(int argc, char ** argv){
	po::options_description desc("Mögliche Optionen");

	desc.add_options()
		("help", "Display help message")
		("k,kunde",po::value<std::string>(), "Name des Kunden")
		("o, ort", po::value<std::string>(), "PLZ und Ort")
		("s, strasse", po::value<std::string>(), "Straße und Hausnummer")
		("fn, firma", po::value<std::string>(), "Firnemnname")
		("fo, firmenort", po::value<std::string>(), "Ort und PLZ der Firma")
		("fs, firmenstrasse", po::value<std::string>(), "Straße und Hausnummer der Firma");
		
	po::variables_map vm;
	po::store(po::parse_command_line(argc,argv, desc), vm);
	po::notify(vm);

	produktDateivorbereiten();
	int produktCounter = 0;
	double gesamtPreis = 0.0;
	int weiter{};
	//Alle Produkte für die Rechnung eingeben.
	do{
		produktCounter += 1;
		std::cin.clear();
		Produkt * p = new Produkt();
		p->writeProduktInfo(produktCounter);
		gesamtPreis += p->preis;
		std::cout << "möchtest du weitermachen? dann gib eine zahl ungleich 0 ein" << std::endl;
		std::cin.sync();
		std::cin.clear();
		std::cin >> weiter;
		std::cin.clear();
	} while ( 0 != weiter );
   
	//Endwerte der Rechnung in Datei schreiben
	std::ofstream gesamt;
	gesamt.open("Rechnung.csv");
	gesamt << gesamtPreis << " " << gesamtPreis*0.19 << " " << gesamtPreis*1.19;
	gesamt.close();
	
	
	std::string befehl{ "xelatex --jobname="};
	befehl.append(vm.count["k"].as<std::string>());
	befehl.append(" '\\def\\name{");
	befehl.append(vm.count["k"].as<std::string>());
	befehl.append("} \\def\\strasse{");
	befehl.append(vm.count["s"].as<std::string>());
	befehl.append("} \\def\\ort{");
	befehl.append(vm.count["o"].as<std::string>());
	befehl.append("}");
	if(vm.count["fs"]&&vm.count["fo"]&&vm.count["fn"]){
		befehl.append(" \\def\\firmenname{");
		befehl.append(vm.count["fs"].as<std::string>());
		befehl.append("} \\def\\firmenort{");
		befehl.append(vm.count["fo"].as<std::string>());
		befehl.append("} \\def\\firmenstrasse{");
		befehl.append(vm.count["fs"].as<std::string>());
		befehl.append("}");
	}
	befehl.append(" \\input{RechnungsTemplate.tex}'");
	char * dir ="~/Rechnung";
	int wechsel_erfolg;
	wechsel_erfolg = chdir(dir);
	if (0 != wechsel_erfolg){
		return 1;
	}
	char * command = befehl.c_str();
	std::system(command);

    	return 0;
}
