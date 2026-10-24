#include <iostream>
#include <set>
#include <string>
#include <fstream>

using namespace std;

struct Data{ 
    string name;
    string phone_num;
    string email;
    int age;

   
    bool operator<(const Data& other) const {
        return name < other.name;
    }
};

class PhoneBook { 
    set<Data> data;

public:

    void add_person(string name, string phone, string email, int age){
        
        Data new_person = {name, phone, email, age};
        
        auto result = data.insert(new_person);
        if (!result.second) {
            cout << "Hiba: Ilyen nevu ember (" << name << ") mar letezik a telefonkonyvben!" << endl;
        }
    }

    void load_phonebook(string filename) {
       
        ifstream fin(filename);
        if (!fin.is_open()) {
            cout << "Hiba a " << filename << " megnyitasakor!" << endl;
            return;
        }

        string name, phone, email;
        int age;
       
        while (fin >> name >> phone >> email >> age) {
            add_person(name, phone, email, age);
        }
        fin.close();
    }

    PhoneBook(){}

    PhoneBook(string filename){ 
        load_phonebook(filename);
    }

    
    auto find_person(string name) {
        Data search_dummy;
        search_dummy.name = name;
        return data.find(search_dummy);
    }

    string get_phone(string name){
        auto it = find_person(name);
        if (it != data.end()) return it->phone_num;
        return "Nincs adat";
    }

    string get_email(string name){
        auto it = find_person(name);
        if (it != data.end()) return it->email;
        return "Nincs adat";
    }

    int get_age(string name){
        auto it = find_person(name);
        if (it != data.end()) return it->age;
        return -1; 
    }

    void remove_person(string name){
       
        Data search_dummy;
        search_dummy.name = name;
        int removed = data.erase(search_dummy);
        if (removed == 0) {
            cout << "Hiba: " << name << " nevu ember nem talalhato, igy nem torolheto!" << endl;
        }
    }

    friend ostream & operator<<(ostream & out, PhoneBook & book){
        /// hogy fout << phone_book és cout << phone_book is mukodjon
        for(auto it = book.data.begin(); it != book.data.end(); ++it){
            out << it->name << "\t" << it->phone_num << "\t" << it->email << "\t" << it->age << "\n";
        }
        return out;
    }

};

int main() {
    /// 0.5 pont
    PhoneBook phone_book("data.txt");
    phone_book.add_person("Gandalf", "+3620/251-4569", "gandalf@lotr.com", 120);
    phone_book.add_person("Frodo", "+3620/569-4569", "frodo@lotr.com", 37);
    phone_book.add_person("ork", "OK", "okr@lotr.com", 3);
    phone_book.add_person("ork", "Hibás", "okr2@lotr.com", 4); /// Itt dobni fog egy hibát, mert "ork" már van!

    /// 0.5 pont
    cout << "Ork telefon " << phone_book.get_phone("ork") << endl;
    cout << "Thor kora " << phone_book.get_age("Thor") << endl;
    cout << "Gandalf email "<< phone_book.get_email("Gandalf") << endl;

    /// 0.5 pont
    phone_book.remove_person("Iron man");
    phone_book.remove_person("Frodo");

    /// 0.5 pont
    ofstream fout("telefonkonyv.txt");
    fout << phone_book << endl;
    fout.close();
    cout << phone_book << endl;

    return 0;
}
