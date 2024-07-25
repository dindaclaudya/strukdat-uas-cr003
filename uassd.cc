#include <iostream>
#include <mysql/mysql.h>
#include <sstream>

using namespace std;

const char* hostname = "127.0.0.1";
const char* user = "root";
const char* pass = "123";
const char* dbname = "uas_crud";
unsigned int port = 31235;
const char* unixsocket = NULL;
unsigned long clientflag = 0;

MYSQL* connect_db() {
    MYSQL* conn = mysql_init(0);
    if (conn) {
        conn = mysql_real_connect(conn, hostname, user, pass, dbname, port, unixsocket, clientflag);
        if (conn) {
            cout << "Terhubung ke database dengan sukses." << endl;
        } else {
            cerr << "Koneksi gagal: " << mysql_error(conn) << endl;
        }
    } else {
        cerr << "mysql_init gagal" << endl;
    }
    return conn;
}

void tambah_perpustakaan(const string& judul, const string& penulis, const string& penerbit, int tahun_terbit) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "INSERT INTO perpustakaan (judul, penulis, penerbit, tahun_terbit) VALUES ('"
              << judul << "', '" << penulis << "', '" << penerbit << "', " << tahun_terbit << ")";
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "INSERT gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "perpustakaan berhasil ditambahkan." << endl;
        }
        mysql_close(conn);
    }
}

void tampilkan_perpustakaan() {
    MYSQL* conn = connect_db();
    if (conn) {
        if (mysql_query(conn, "SELECT * FROM perpustakaan")) {
            cerr << "SELECT gagal: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "mysql_store_result gagal: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << row[0] << ", Judul: " << row[1] << ", Penulis: " << row[2]
                 << ", Penerbit: " << row[3] << ", Tahun Terbit: " << row[4] << endl;
        }

        mysql_free_result(res);
        mysql_close(conn);
    }
}

void update_perpustakaan(int perpustakaan_id, const string& judul, const string& penulis, const string& penerbit, int tahun_terbit) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "UPDATE perpustakaan SET judul = '" << judul << "', penulis = '" << penulis << "', penerbit = '" << penerbit
              << "', tahun_terbit = " << tahun_terbit << " WHERE id = " << perpustakaan_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "UPDATE gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "perpustakaan berhasil diperbarui." << endl;
        }
        mysql_close(conn);
    }
}

void hapus_perpustakaan(int perpustakaan_id) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "DELETE FROM perpustakaan WHERE id = " << perpustakaan_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "DELETE gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "perpustakaan berhasil dihapus." << endl;
        }
        mysql_close(conn);
    }
}

int main() {
    int pilihan;
    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Tambah perpustakaan\n";
        cout << "2. Tampilkan Semua perpustakaan\n";
        cout << "3. Perbarui perpustakaan\n";
        cout << "4. Hapus perpustakaan\n";
        cout << "5. Keluar\n";
        cout << "Masukkan pilihan: ";
        cin >> pilihan;

        if (pilihan == 1) {
            string judul, penulis, penerbit;
            int tahun_terbit;
            cout << "Masukkan judul: ";
            cin.ignore();
            getline(cin, judul);
            cout << "Masukkan penulis: ";
            getline(cin, penulis);
            cout << "Masukkan penerbit: ";
            getline(cin, penerbit);
            cout << "Masukkan tahun terbit: ";
            cin >> tahun_terbit;
            tambah_perpustakaan(judul, penulis, penerbit, tahun_terbit);
        } else if (pilihan == 2) {
            tampilkan_perpustakaan();
        } else if (pilihan == 3) {
            int perpustakaan_id;
            string judul, penulis, penerbit;
            int tahun_terbit;
            cout << "Masukkan ID perpustakaan yang akan diperbarui: ";
            cin >> perpustakaan_id;
            cin.ignore();
            cout << "Masukkan judul baru: ";
            getline(cin, judul);
            cout << "Masukkan penulis baru: ";
            getline(cin, penulis);
            cout << "Masukkan penerbit baru: ";
            getline(cin, penerbit);
            cout << "Masukkan tahun terbit baru: ";
            cin >> tahun_terbit;
            update_perpustakaan(perpustakaan_id, judul, penulis, penerbit, tahun_terbit);
        } else if (pilihan == 4) {
            int perpustakaan_id;
            cout << "Masukkan ID perpustakaan yang akan dihapus: ";
            cin >> perpustakaan_id;
            hapus_perpustakaan(perpustakaan_id);
        } else if (pilihan == 5) {
            break;
        } else {
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }
    }
    return 0;
}