#include <iostream>
#include <stdlib.h>
#include <conio.h>//untuk getch
#include <cmath> //untuk sqrt
#include <iomanip>//untuk setw
#include <string>
#include <windows.h>
using namespace std;
#include <fstream>
#include <vector>//untuk vektor
#include <cctype>
const int rows = 9;
const int seatsPerRow = 6;
int MenuUser();
int main();
void MenuUtamaUser();
struct maskapai {
    int id_tiket;
    string tujuan;//alamat tujuan
    string tanggal;//jumlah tanggal
    string waktu;//waktu penerbangan
    int harga;//harga pemesanan
    string nama_pesawat;//nama pesawat
};
struct pesanan {
    maskapai tiket;
    pesanan* next;
};
struct node {
    maskapai mask;
    pesanan* pesanans;
    node* next;
    node* prev;
};
struct node* head = NULL;
struct node* last = NULL;
vector<vector<char>> planeCSV(rows, vector<char>(seatsPerRow, 'O'));
vector<string> csvFileNames = {"plane1.csv", "plane2.csv", "plane3.csv"};
int selectedFile = 0; // Indeks file CSV yang aktif
void gotoxy(int x, int y){//untuk letak/titik koordinat
    COORD c;
    c.X=x;
    c.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}
void saveToCSV(string fileName) {//simpan ke CSv
    ofstream outputFile(fileName);

    if (outputFile.is_open()) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < seatsPerRow; j++) {
                outputFile << planeCSV[i][j];
                if (j < seatsPerRow - 1) {
                    outputFile << ",";
                }
            }
            outputFile << endl;
        }
        outputFile.close();
        cout << "Data kursi telah disimpan ke dalam " << fileName << endl;
    } else {
        cout << "Gagal menyimpan data kursi ke dalam " << fileName << endl;
    }
}

void loadFromCSV(string fileName) {//load csv
    ifstream inputFile(fileName);

    if (inputFile.is_open()) {
        for (int i = 0; i < rows; i++) {
            string line;
            getline(inputFile, line);
            for (int j = 0; j < seatsPerRow; j++) {
                if (line[j * 2] == 'O' || line[j * 2] == 'X') {
                    planeCSV[i][j] = line[j * 2]; // mengupdate vector planeCSV
                }
            }
        }
        inputFile.close();
        cout << "Data kursi telah dimuat dari " << fileName << endl;
    } else {
        cout << "File " << fileName << " tidak ditemukan. Menggunakan data awal." << endl;
    }
}
class UserNode {//anggap aja struct untuk kebutuhan login
public:
    string username;
    string password;
    string role;
    UserNode* next;

    UserNode(const string& uname, const string& pass, const string& r)
        : username(uname), password(pass), role(r), next(nullptr) {}
};
class UserList {//untuk Login juga
private:
    UserNode* head;

public:
    UserList() : head(nullptr) {}

    void addUser(const string& username, const string& password, const string& role) {
        // Periksa apakah nama pengguna sudah ada dalam daftar
        UserNode* current = head;
        while (current) {
            if (current->username == username) {
                cout << "Nama pengguna sudah ada. Registrasi gagal." << endl;
                return;
            }
            current = current->next;
        }

        // Jika nama pengguna belum ada, tambahkan pengguna baru ke dalam daftar
        UserNode* newUser = new UserNode(username, password, role);
        newUser->next = head;
        head = newUser;

        cout << "Pengguna berhasil terdaftar!" << endl;
    }

    UserNode* login(const string& username, const string& password) {
        // Admin login without registration
        if (username == "admin" && password == "adminpass") {
            return new UserNode("admin", "adminpass", "admin");
        }

        // Implementasi login
        UserNode* current = head;
        while (current) {
            if (current->username == username && current->password == password) {
                return current;
            }
            current = current->next;
        }

        return nullptr;
    }

    void saveToCSV(const string& fileName) {
        ofstream outputFile(fileName);

        if (outputFile.is_open()) {
            UserNode* current = head;
            while (current) {
                outputFile << current->username << "," << current->password << "," << current->role << endl;
                current = current->next;
            }

            outputFile.close();
            cout << "Data pengguna telah disimpan ke dalam " << fileName << endl;
        } else {
            cout << "Gagal menyimpan data pengguna ke dalam " << fileName << endl;
        }
    }

    void loadFromCSV(const string& fileName) {
        ifstream inputFile(fileName);

        if (inputFile.is_open()) {
            clearList();

            string username, password, role;
            while (getline(inputFile, username, ',')) {
                getline(inputFile, password, ',');
                getline(inputFile, role);

                addUser(username, password, role);
            }

            inputFile.close();
            cout << "Data pengguna telah dimuat dari " << fileName << endl;
        } else {
            cout << "File " << fileName << " tidak ditemukan. Membuat daftar pengguna kosong." << endl;
        }
    }

    void clearList() {
        while (head) {
            UserNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    ~UserList() {
        clearList();
    }
};

void displayPlane() {//untuk display/tampilkan pesawat
    cout << "   A  B  C  D  E  F" << endl;
    for (int i = 0; i < rows; i++) {
        cout << i + 1 << "  ";
        for (int j = 0; j < seatsPerRow; j++) {
            if (planeCSV[i][j] == 'X') {
                cout << "X  ";
            } else {
                cout << "O  ";
            }
        }
        cout << endl;
    }
}

void resetCSV() {//untuk reset kursinya
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < seatsPerRow; j++) {
            planeCSV[i][j] = 'O';
        }
    }
    cout << "Data kursi telah di-reset." << endl;
}

bool isSeatValid(int row, int seat) {//apakah valid
    return row >= 1 && row <= rows && seat >= 0 && seat < seatsPerRow;
}

bool isSeatAvailable(int row, int seat) {//apakah kursi kosong?
    return planeCSV[row - 1][seat] == 'O';
}

void reserveSeat(int row, int seat) {
    if (isSeatValid(row, seat) && isSeatAvailable(row, seat)) {
        planeCSV[row - 1][seat] = 'X';
    }
}

void handleInvalidInput() {//error handle input
    cout << "Input tidak valid. Silakan coba lagi." << endl;
}
int MenuUser() {
    // Menampilkan isi dari plane1.csv saat program pertama kali dijalankan
    loadFromCSV(csvFileNames[selectedFile]);

    string choice;
    int row, seat;

    while (true) {
        displayPlane();
        cout << "Pemesanan Contoh (B2) ";
        cout << "Pilih file CSV (0-" << csvFileNames.size() - 1 << "), 'R' untuk mereset, 'S' untuk simpan, atau 'Q' untuk keluar: ";
        cin >> choice;

        if (choice == "Q" || choice == "q") {
            saveToCSV(csvFileNames[selectedFile]);
            break;
        } else if (choice == "R" || choice == "r") {
            resetCSV();
        } else if (choice == "S" || choice == "s") {
            saveToCSV(csvFileNames[selectedFile]);
        } else if (isdigit(choice[0])) {
            int fileIndex = stoi(choice);
            if (fileIndex >= 0 && fileIndex < csvFileNames.size()) {
                selectedFile = fileIndex;
                loadFromCSV(csvFileNames[selectedFile]);
            } else {
                cout << "Indeks file tidak valid." << endl;
            }
        } else if (isalpha(choice[0]) && isdigit(choice[1])) {
            // Pastikan input sesuai dengan format (contoh: B2)
            seat = toupper(choice[0]) - 'A';
            row = choice[1] - '0';

            if (row >= 1 && row <= rows && seat >= 0 && seat < seatsPerRow) {
                if (!isSeatAvailable(row, seat)) {
                    cout << "Kursi " << row << choice[0] << " sudah terisi. Pilih kursi lain." << endl;
                } else {
                    reserveSeat(row, seat);
                    cout << "Kursi " << row << choice[0] << " telah terisi." << endl;
                }
            } else {
                handleInvalidInput();
            }
        } else {
            handleInvalidInput();
        }
    }

    return 0;
}
void menu();
void show();
int main();
bool kosong() {
    return head == NULL;
}
void tampilkanData(const maskapai& data) {
    cout << "ID: " << data.id_tiket << endl;
    cout << "Tujuan: " << data.tujuan << endl;
    cout << "Tanggal penerbangan: " << data.tanggal << endl;
    cout << "Waktu: " << data.waktu << endl;
    cout << "Harga tiket: " << data.harga << endl;
    cout << "Nama pesawat: " << data.nama_pesawat << endl;
    cout << "=========================================" << endl;
}

void tampilkanSemuaData() {
    node* current = head;
    while (current) {
        tampilkanData(current->mask);
        current = current->next;
    }
}

void hapusTiketDariDaftar(int idTiket) {
    if (kosong()) {
        cout << "Tidak ada tiket yang tersedia." << endl;
        return;
    }

    node* current = head;
    node* prev = nullptr;

    while (current) {
        if (current->mask.id_tiket == idTiket) {
            // Hapus tiket dari daftar utama
            if (prev) {
                prev->next = current->next;
            } else {
                head = current->next;
            }

            delete current;
            cout << "Tiket dengan ID " << idTiket << " telah dihapus dari daftar utama." << endl;
            return;
        }

        prev = current;
        current = current->next;
    }

    // ID tiket tidak ditemukan
    cout << "Tiket dengan ID " << idTiket << " tidak ditemukan." << endl;
}

void pesanTicket() {
    if (kosong()) {
        cout << "Tidak ada tiket yang tersedia." << endl;
        return;
    }
    string idPesanstr;
    int idPesan;
    cout << "Masukkan ID tiket yang ingin Anda pesan: ";
    cin >> idPesanstr;
     try {
            idPesan =  stoi(idPesanstr);
        }
        catch (const  invalid_argument& e) {
            gotoxy(48,18);
            cout << "Input tidak valid. Masukkan angka yang valid." << endl;
            getch();
            main();
        }
    hapusTiketDariDaftar(idPesan);

    node* current = head;
    while (current) {
        if (current->mask.id_tiket == idPesan) {
            // Pesan tiket (tindakan sesuai kebutuhan)
            cout << "Anda telah berhasil memesan tiket dengan ID " << idPesan << "." << endl;

            // Menambahkan tiket ke daftar pesanan pengguna
            pesanan* pesan = new pesanan{ current->mask, nullptr };
            if (current->pesanans == nullptr) {
                current->pesanans = pesan;
            } else {
                pesanan* temp = current->pesanans;
                while (temp->next) {
                    temp = temp->next;
                }
                temp->next = pesan;
            }
            return;
        }
        current = current->next;
    }

    // ID tiket tidak ditemukan
    cout << "Tiket dengan ID " << idPesan << " tidak ditemukan." << endl;
}
void menutiketUser() {
    int choice;
    string strchoice;
    do {
        system("cls");
        // Display menu options
        gotoxy(45,20);
        cout << "Menu User:" << endl;
        gotoxy(45,21);
        cout << "1. Pesan Ticket" << endl;
        gotoxy(45,22);
        cout << "2. Lihat Semua Ticket" << endl;     
        gotoxy(45,23);
        cout << "4. Kembali" << endl;        
        gotoxy(45,24);
        // Get user input
        cout << "Pilihan Anda: ";
        cin >> strchoice;
          try{
            choice=stoi(strchoice);
        }
    catch (const  invalid_argument& e) {
        gotoxy(48,23);
    cout << "Input tidak valid. Masukkan angka yang valid." << endl;
    getch();
    system("cls");
    main();
    }

        // Perform actions based on user input
        switch (choice) {
            case 1:
                // Pesan tiket
                pesanTicket();
                break;

            case 2:
                // Lihat semua tiket
                tampilkanSemuaData();
                break;
            case 3:
                // Keluar dari menu
                cout << "Kembali ke menu sebelumnya." << endl;
                break;

            default:
                // Handle invalid input
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }

    } while (strchoice != "3"); // Continue the loop until the user chooses to go back
}


int lastTicketID = 0; // Global variable to track the last assigned ticket ID

void autoIncrementID(maskapai &mask) {
    mask.id_tiket = ++lastTicketID;
}
void adminMenu(UserNode* adminUser) {
    cout << "Selamat datang, Admin " << adminUser->username << "!" << endl;

}

// Fungsi untuk menampilkan menu user
void menuuser(UserNode* user) {
    system("cls");
    cout << "Selamat datang, " << user->username << "!" << endl;
    MenuUtamaUser();
    // cout<<"\nIni adalah data yang sudah ada"<<endl;
    getch();
    system("cls");
    main();
}
void MenuUtamaUser(){
        string choice_str;
    do{
    int choice;
    gotoxy(45,20);
    cout<<"====================="<<endl;
    gotoxy(45,21);
    cout<<"Pilihan"<<endl;    
    gotoxy(45,22);
    cout<<"1.Pesan Kursi"<<endl;    
    gotoxy(45,23);
    cout<<"2.Tiket"<<endl;    
    gotoxy(45,24);
    cout<<"3.Kembali"<<endl;    
    gotoxy(45,25);
    cout<<"===================="<<endl;
    gotoxy(45,26);
    cout<<"Mau pilih apa ? : ";cin>>choice_str;
    try{
        choice=stoi(choice_str);
    } 
     catch (const  invalid_argument& e) {
            gotoxy(45,27);
            cout << "Input tidak valid. Masukkan angka yang valid." << endl;
            getch();
            continue;
        }
    switch(choice){
        case 1: MenuUser();
        break;
        case 2: menutiketUser();
        break;
        case 3:
        main();
        break;
    }
    }while(choice_str!="3");
    cout<<"Salah oi"<<endl;
}


int main() {
    UserList userList;
    string choice_str;
    // Tambahkan beberapa pengguna contoh ke dalam daftar
    userList.addUser("admin", "adminpass", "admin");
    userList.addUser("user1", "userpass", "user");
    userList.addUser("user2", "userpass", "user");
    userList.loadFromCSV("users.csv");
    while (true) {
        system("cls");
        int choice;
        gotoxy(48,16);
        cout<<"=============="<<endl;
        gotoxy(48,17);
        cout << "Menu:"<<endl;
        gotoxy(48,18);
        cout<<"1. Login "<<endl;
        gotoxy(48,19);
        cout<<"2. Register "<<endl;        
        gotoxy(48,20);
        cout<<"3. Keluar "<<endl;
        gotoxy(48,21);
        cout<<"=============="<<endl;
        gotoxy(48,22);
        cout<<"Pilih: ";
        cin >> choice_str;
        try{
            choice=stoi(choice_str);
        }
    catch (const  invalid_argument& e) {
        gotoxy(48,23);
    cout << "Input tidak valid. Masukkan angka yang valid." << endl;
    getch();
    system("cls");
    main();
    }
        if (choice == 1) { // Login
        gotoxy(48,23);
            cout << "Masukkan username: ";
            string username, password;cin.ignore();
            getline(cin,username);
            if(username==""){
                gotoxy(48,23);
                cout<<"Harap diisi"<<endl;
                getch();
                system("cls");
                main();
            }
            gotoxy(48,24);
            cout << "Masukkan password: ";
            getline(cin,password);
               if(password==""){
                gotoxy(48,24);
                cout<<"Harap diisi"<<endl;
                getch();
                system("cls");
                main();
            }

            UserNode* loggedInUser = userList.login(username, password);
            if (loggedInUser) {
                if (loggedInUser->role == "admin") {//jika login sebagai admin
                    menu();
                } else {//jika login sebagai user
                    menuuser(loggedInUser);
                }
            } else {//jika gagal
                gotoxy(48,25);
                cout << "Login gagal. Coba lagi." << endl;
                getch();
                system("cls");
            }
        } else if (choice == 2) { // Register (menambahkan pengguna baru)
            gotoxy(48,23);
            cout << "Masukkan username: ";
            string username, password;cin.ignore();
            getline(cin,username);
             if(username==""){
                cout<<"Harap diisi"<<endl;
                getch();
                system("cls");
                main();
            }
            gotoxy(48,24);
            cout << "Masukkan password: ";
            getline(cin,password);
               if(password==""){
                cout<<"Harap diisi"<<endl;
                getch();
                system("cls");
                main();
            }
            userList.addUser(username, password, "user");
            gotoxy(48,25);
            userList.saveToCSV("users.csv");
            cout << "Pengguna berhasil terdaftar!" << endl;
            getch();
            system("cls");
        } else if (choice == 3) { // Keluar
            break;
        }
    }

    return 0;
}

void updateID() {//Agar si id_tiket menjadi Auto increment
    node* temp = head;
    int index = 1;

    while (temp != NULL) {
        temp->mask.id_tiket = index++;
        temp = temp->next;
    }
}

void Tambah(maskapai mask){
    string harga_str;
    gotoxy(50,20);
    cout<<"Masukkan tujuan : ";cin.ignore();getline(cin,mask.tujuan);fflush(stdin);
    if(mask.tujuan==""){
        system("cls");
        gotoxy(50,20);
        cout<<"Tidak boleh dikosongkan"<<endl;
        return;
    }
        gotoxy(50,21);
  cout<<"Masukkan tanggal : ";getline(cin,mask.tanggal);fflush(stdin);
    if(mask.tanggal==""){
        system("cls");
        gotoxy(50,20);
        cout<<"Tidak boleh dikosongkan"<<endl;
        return;
    }    
    gotoxy(50,22);
    cout<<"Masukkan waktu :";getline(cin,mask.waktu);fflush(stdin);
    if(mask.waktu==""){
        system("cls");    
        gotoxy(50,20);
        cout<<"Tidak boleh dikosongkan"<<endl;
        return;
    }
    gotoxy(50,23);
    cout<<"Masukkan harga : ";cin>>harga_str;fflush(stdin);
    try{
        mask.harga=stoi(harga_str);
    }
    catch (const  invalid_argument& e) {
    gotoxy(50,20);
    cout << "Input tidak valid. Masukkan angka yang valid." << endl;
    getch();
    menu();
    }
        gotoxy(50,24);
    cout<<"Masukkan nama pesawat :";getline(cin, mask.nama_pesawat);fflush(stdin);
    if(mask.nama_pesawat==""){
        system("cls");    
        gotoxy(50,20);
        cout<<"TIdak Boleh dikosongkan"<<endl;
        return;
    }
    if (kosong()) {//Jika menjadi data pertama
        node* temp = new node;//buat node baru
        temp->mask = mask;//transversing
        temp->next = NULL;//next nya NULL
        temp->prev = NULL;//prevnya juga NULL
        head = temp;//head dan last adalah temp karena dia baru 1 data
        last = temp;
        updateID();
        gotoxy(45,26);
        cout << "Silahkan tekan enter untuk melihat hasil add data" << endl;
        getch();
        system("cls");
        gotoxy(45,20);
        cout<<"========================================="<<endl;
        gotoxy(45,21);
        cout<<  "ID :" <<last->mask.id_tiket<<endl;
        gotoxy(45,22);
        cout << "Tujuan : " << last->mask.tujuan << endl;
        gotoxy(45,23);
        cout << "Tanggal penerbangan : " << last->mask.tanggal << endl;
        gotoxy(45,24);
        cout << "Waktu : " << last->mask.waktu << endl;
        gotoxy(45,25);
        cout << "Harga tiket : " << last->mask.harga << endl;
        gotoxy(45,26);
        cout << "Nama pesawat : " << last->mask.nama_pesawat << endl;
        gotoxy(45,27);
        cout<<"========================================="<<endl;
        gotoxy(45,28);
    } else {//jika dia bukan satu satunya data yang ada 
        node* temp = new node;//buat new node
        temp->mask = mask;//transversing
        temp->next = NULL;//next nya null karena dia akan dijadikan node terakhir
        temp->prev = last;  // Link 'prev' to the previous node
        last->next = temp;//node baru akan dijadikan last,last berpindah dari node sebelum dia ke dia
        last = temp;
        updateID();
        gotoxy(45,28);
        cout << "Silahkan tekan enter untuk melihat hasil add data" << endl;
        getch();
        system("cls");
        gotoxy(45,20);
        cout<<"========================================="<<endl;
        gotoxy(45,21);
        cout<<  "ID :" <<last->mask.id_tiket<<endl;
        gotoxy(45,22);
        cout << "Tujuan : " << last->mask.tujuan << endl;
        gotoxy(45,23);
        cout << "Tanggal penerbangan : " << last->mask.tanggal << endl;
        gotoxy(45,24);
        cout << "Waktu : " << last->mask.waktu << endl;
        gotoxy(45,25);
        cout << "Harga tiket : " << last->mask.harga << endl;
        gotoxy(45,26);
        cout << "Nama pesawat : " << last->mask.nama_pesawat << endl;
        gotoxy(45,27);
        cout<<"========================================="<<endl;
        gotoxy(45,28);
    } autoIncrementID(mask); 
}
int jumpSearch(maskapai mask) {
    int cari_id;
    cout<<"Masukkan ID yang anda cari : ";cin.ignore(); cin>>cari_id;
    if (kosong()) {
        cout << "Tidak ada data dalam list" << endl;
        return -1; // Tidak ada data, kembalikan nilai indeks negatif
    }

    int total = 0;  // Hitung total elemen dalam list, Anda perlu menghitungnya sesuai dengan struktur data Anda
    int jump = sqrt(total); // Hitung ukuran loncatan

    // Inisialisasi pointer untuk melompati bagian awal
    node* current = head;
    int index = 1;

    // Lakukan pencarian lompatan
    while (current != NULL && current->mask.id_tiket < cari_id) {
        // Loncat ke blok berikutnya
        index += jump;
        current = current->next;

        if (current == NULL) {
            break;
        }
    }

    // Lakukan pencarian linear di dalam blok
    while (current != NULL && current->mask.id_tiket != cari_id) {
        current = current->next;
        index++;
    }
    //jika tidak kosong dan inputan id_tiket sesuai alias ada di data
    if (current != NULL && current->mask.id_tiket == cari_id) {
        cout << "Nama Maskapai '" << cari_id << "' ditemukan pada indeks: " << index << endl;
        cout <<  "ID :" <<current->mask.id_tiket<<endl;
        cout << "Tujuannya ke : " << current->mask.tujuan << endl;
        cout << "Tanggal terbang : " << current->mask.tanggal << endl;
        cout << "Jam terbang : " << current->mask.waktu << endl;
        cout << "Harga tiket : " << current->mask.harga << endl;
        cout << "Nama Maskapai : " << current->mask.nama_pesawat <<"\n"<< endl;
        return index;
    } else {//jika idnya tidak ada
        cout << "Nama Maskapai'" << cari_id << "' tidak ditemukan." << endl;
        return -1; // Buku tidak ditemukan, kembalikan nilai indeks negatif
    }
}
void show();
void deleteFirst(maskapai mask) {
      if (kosong()) {//Jika data kosong,apa yang dihapus?
        gotoxy(45,20);
        cout << "Tidak ada Data dalam list" << endl;
    } else if (head == last) {//Jika hanya ada 1 data
        delete head;//delete
        head = NULL;//head jadi null begitu pula last karena datanya jadi kosong
        last = NULL;
        gotoxy(45,20);
        cout << "Data pertama berhasil dihapus" << endl;
    } else {//Jika ada lebih dari 1 data
        node* temp = head;//kita cari si head alias data pertama
        head = head->next;//Jadikan data setelahnya menjadi head baru
        head->prev = NULL;  // prev dari data yang dijadikan head baru dibuat null,karena sebelumnya prevnya menunjuk ke head lama
        delete temp;//delete
        gotoxy(45,20);
        cout << "Data pertama berhasil dihapus" << endl;
    }
}



void swapNodes(node* a, node* b) {
        // Tukar data antara dua node tanpa mengubah node-nya
    maskapai temp = a->mask;
    a->mask = b->mask;
    b->mask = temp;
}

int compareLainnya(maskapai mask3, maskapai mask4, bool ascend) {
    // Bandingkan maskapai berdasarkan judul
    if (ascend) {
        return mask3.nama_pesawat.compare(mask4.nama_pesawat);
    } else {
        return mask4.nama_pesawat.compare(mask3.nama_pesawat);
    }
}

node* partisi(node* low, node* high, bool ascend) {
    maskapai pivot = high->mask;//buat pivot
    node* i = low->prev;

    for (node* j = low; j != high; j = j->next) {
        if (compareLainnya(j->mask, pivot, ascend) < 0) {
            if (i == nullptr) {
                i = low;
            } else {
                i = i->next;
            }
            swapNodes(i, j);
        }
    }

    if (i == nullptr) {
        i = low;
    } else {
        i = i->next;
    }
    swapNodes(i, high);
    return i;
}

    void quick(node* low, node* high, bool ascend) {
        if (low && high && low != high && low != high->next) {
        //low: Ini adalah pointer ke node pertama atau node terendah dalam bagian dari linked list yang sedang diurutkan.
        //high: Ini adalah pointer ke node terakhir atau node tertinggi dalam bagian dari linked list yang sedang diurutkan.
            node* partitionNode = partisi(low, high, ascend);
            quick(low, partitionNode->prev, ascend);
            quick(partitionNode->next, high, ascend);
        }
    }


    void sortDescend() {
        if (kosong()) {
             cout << "Tidak ada data maskapai dalam list" <<  endl;
            return;
        }
        quick(head, last, false);//Jika False maka dia akan jadi descending
    }

    void sortAscend() {
        // if (kosong()) {
        //      cout << "Tidak ada data maskapai dalam list" <<  endl;
        //     return;
        // }
        quick(head, last, true);//Jika True maka dia akan jadi ascending
    }

int compareBooks(maskapai mask1, maskapai mask2, bool ascending) {
    // Comparing
    if (ascending) {
        return mask1.tanggal.compare(mask2.tanggal);
    } else {
        return mask2.tanggal.compare(mask1.tanggal);
    }
}

node* partition(node* low, node* high, bool ascending) {
    maskapai pivotmaskapai = high->mask;//pivot
    node* i = low->prev;

    for (node* j = low; j != high; j = j->next) {
        if (compareBooks(j->mask, pivotmaskapai, ascending) < 0) {
            if (i == nullptr) {
                i = low;
            } else {
                i = i->next;
            }
            swapNodes(i, j);
        }
    }

    if (i == nullptr) {
        i = low;
    } else {
        i = i->next;
    }
    swapNodes(i, high);
    return i;
}


    void quickSort(node* low, node* high, bool ascending) {
        if (low && high && low != high && low != high->next) {
            node* partitionNode = partition(low, high, ascending);
            quickSort(low, partitionNode->prev, ascending);
            quickSort(partitionNode->next, high, ascending);
        }
    }


    void sortDescending() {
        if (kosong()) {
             cout << "Harap Masukkan terlebih dahulu" <<  endl;
            return;
        }
        quickSort(head, last, false);
    }

    void sortAscending() {
        // if (kosong()) {
        //      cout << "Harap Masukkan terlebih dahulu" <<  endl;
        //     return;
        // }
        quickSort(head, last, true);
    }
    
void update(maskapai mask) {
    string waktu_sementara;
    string tanggal_sementara;
    string harga_sementara;
    string tujuan_sementara;
    string id_sementara;
    int id_baru;
    string pesawat_sementara;
    show();
    if(kosong()){
        getch();
        system("cls");
        menu();
    }
    cout << "Masukkan id_tiket yang ingin diubah: ";cin.ignore();cin>>id_sementara;
    if (id_sementara=="") {
        cout << "Tolong serius dalam menginput" << endl;
        getch();
        menu();
    }
    node* temp = head;
    bool found = false;
    try
    {
        id_baru=stoi(id_sementara);
    }
    catch(const invalid_argument& e )
    {
        cout << "Input tidak valid. Masukkan angka yang valid." << endl;
            getch();
            menu();
    }
    
    while (temp != NULL) {
        if (temp->mask.id_tiket == id_baru) {
             updateID();
        cout<<"Masukkan tujuan baru : ";cin.ignore();getline(cin,tujuan_sementara);fflush(stdin);
        if(tujuan_sementara==""){
            cout<<"Tidak boleh dikosongkan"<<endl;
            return;
        }
        cout<<"Masukkan tanggal nya : ";getline(cin,tanggal_sementara);fflush(stdin);
        if(tanggal_sementara==""){
            cout<<"Tidak boleh dikosongkan"<<endl;
            return;
        } 
        cout<<"Masukkan Waktu nya : ";getline(cin,waktu_sementara);fflush(stdin);
        if(waktu_sementara==""){
            cout<<"Tidak boleh dikosongkan"<<endl;
            return;
        }
        cout<<"Masukkan Harga nya : ";cin>>harga_sementara;fflush(stdin);
        try{
            temp->mask.harga=stoi(harga_sementara);
        }
        catch(const invalid_argument& e){
            cout << "Input tidak valid. Masukkan angka yang valid." << endl;
            getch();
            menu();
        }
        cout<<"Masukkan nama pesawat : ";getline(cin,pesawat_sementara);fflush(stdin);
        if(pesawat_sementara==""){
            cout<<"Tidak boleh dikosongkan"<<endl;
            return;
        }
             
            temp->mask.tujuan=tujuan_sementara;
            temp->mask.waktu=waktu_sementara;
            temp->mask.tanggal=tanggal_sementara;
            temp->mask.nama_pesawat=pesawat_sementara;
            system("cls");
            cout << "Perubahan " << endl;
            cout<<"========================================="<<endl;
            cout<<"ID : "<<temp->mask.id_tiket<<endl;
            cout<<"Tujuan : "<< temp->mask.tujuan<<endl;
            cout<<"Tanggal : "<< temp->mask.tanggal<<endl;
            cout<<"Waktu : "<< temp->mask.waktu<<endl;
            cout<<"Harga Tiket : "<< temp->mask.harga<<endl;
            cout<<"Nama Pesawat : "<< temp->mask.nama_pesawat<<endl;        
            cout<<"========================================="<<endl;
            found = true;
            cout<<"Press any key to continue"<<endl;
            getch();
            break;
        }
        temp = temp->next;
    }if (!found) {
        cout << "Data dengan id_tiket '" << id_sementara << "' tidak ditemukan." << endl;
        cout<<"Press any key to continue"<<endl;
        getch();
        menu();
    }
}
void show() {
    if (kosong()) {
        gotoxy(50,19);
        cout << "Tidak ada Data dalam list" << endl;
    }
    else {
        int total = 0;
        node* temp = head;
        gotoxy(45,18);
        cout<<"=============Data Penerbangan============="<<endl;
        while (temp != NULL) {
            total += 1;      
        gotoxy(45,19);  
        cout<<"========================================="<<endl;
        gotoxy(45,20);
        cout<<"Data Ke : "<<total<<endl;   
        gotoxy(45,21);    
        cout<<  "ID : "<<temp->mask.id_tiket<<endl;
        gotoxy(45,22);
        cout << "Tujuan : " << temp->mask.tujuan << endl;        
        gotoxy(45,23);
        cout << "Tanggal penerbangan : " << temp->mask.tanggal << endl;        
        gotoxy(45,24);
        cout << "Waktu : " << temp->mask.waktu << endl;        
        gotoxy(45,25);
        cout << "Harga tiket : " << temp->mask.harga << endl;        
        gotoxy(45,26);
        cout << "Nama pesawat : " << temp->mask.nama_pesawat << endl;         
        gotoxy(45,27);
        cout<<"========================================="<<endl;
        temp = temp->next;
        }
    }
}
string centerText(const string& text, int width) {//fungsi untuk mengatur letak text
    int length = text.length();
    int padding = (width - length) / 2;
    return string(padding, ' ') + text + string(padding, ' ');
}

void menu(){
    string pilihan_sementara;//variabel untuk di stoikan agar ketika user memasukan huruf,dia tidak bisa
    maskapai mask;
    int index;
    string choice;
    int row, seat;
    do{
    system("cls");
    gotoxy(48,6);
    cout<<"================================="<<endl;
    gotoxy(48,7);    
    cout<<"| SELAMAT DATANG                |"<<endl;    
    gotoxy(48,8);
    cout<<"| 1.Tambah                      |"<<endl;    
    gotoxy(48,9);
    cout<<"| 2.Hapus                       |"<<endl;
    gotoxy(48,10);
    cout<<"| 3.Edit                        |"<<endl;    
    gotoxy(48,11);
    cout<<"| 4.Show Sorting tanggal        |"<<endl;    
    gotoxy(48,12);
    cout<<"| 5.Show Sorting nama           |"<<endl;    
    gotoxy(48,13);
    cout<<"| 6.Searching                   |"<<endl;    
    gotoxy(48,14);
    cout<<"| 7.Log Out                     |"<<endl;    
    gotoxy(48,15);
    cout<<"| 8.Show Kursi                  |"<<endl;
    gotoxy(48,16);
    cout<<"| 9.Exit                        |"<<endl;  
    gotoxy(48,17);
    cout<<"================================="<<endl;    
    gotoxy(48,18);
    cout<<"Masukkan pilihan anda : ";cin>>pilihan_sementara;
       int pilihan;
        try {
            pilihan =  stoi(pilihan_sementara);
        }
        catch (const  invalid_argument& e) {
            gotoxy(48,18);
            cout << "Input tidak valid. Masukkan angka yang valid." << endl;
            getch();
            continue;
        }
    switch(pilihan){
        case 1 : 
        system("cls");
        Tambah(mask);
        updateID();
        sortAscend();
        break;
        case 2 :
        system("cls");
        deleteFirst(mask);
        break;
        case 3:
        system("cls");
        update(mask);
        break;
        case 4 :
        system("cls");
        sortAscending();
        show();
        break;
        case 5 :
        system("cls");
        sortAscend();
        show();
        break;
        case 6 :
        updateID();
        system("cls");
        jumpSearch(mask);
        break;
        case 7 :
        system("cls");
        main();
        break;
        case 8 :
{
    bool backToMenu = false;
    do
    {
        system("cls");
        displayPlane();
        loadFromCSV(csvFileNames[selectedFile]);
        cout << "Pilih file CSV (0-" << csvFileNames.size() - 1 << "), 'R' untuk mereset, atau 'Q' untuk keluar: ";
        cin >> choice;
        if (isdigit(choice[0]))
        {
            int fileIndex = stoi(choice);
            if (fileIndex >= 0 && fileIndex < csvFileNames.size())
            {
                selectedFile = fileIndex;
                loadFromCSV(csvFileNames[selectedFile]);
                displayPlane();
            }
            else
            {
                cout << "Indeks file tidak valid." << endl;
            }
        }
        else if (toupper(choice[0]) == 'R')
        {
            resetCSV(); // Reset the data without saving
            saveToCSV(csvFileNames[selectedFile]); // Save the data immediately after resetting
            displayPlane(); // Update the display after resetting
            cout << "Data berhasil direset dan disimpan." << endl;
        }
        else if (toupper(choice[0]) == 'Q')
        {
            backToMenu = true;
        }
        else
        {
            cout << "Input tidak valid." << endl;
        }

        if (!backToMenu && (toupper(choice[0]) != '1'))
        {
            cout << "\t\t\nTekan Enter untuk melanjutkan...";
            cin.ignore();
            cin.get();
        }

    } while (!backToMenu);
}
break;
    case 9:
    system("cls");
     gotoxy(48,20);
    cout<<"Anda keluar program"<<endl;
    exit(0);
    break;
        default:
        gotoxy(48,19);
       cout<<"Pilihan diluar nalar"<<endl;
        break;
        } 
     string kalimat = "Tekan Enter untuk melanjutkan...";
    int lebar_terminal = 40; 
    cout << centerText(kalimat, lebar_terminal) << endl;
        cin.ignore();
        cin.get();
        menu();
    }while( pilihan_sementara !="9");
    cout<<"Salah oi"<<endl;
}