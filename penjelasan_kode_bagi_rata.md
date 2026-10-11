# Penjelasan Lengkap Kode Sumber: bagi_rata.c
## Proyek Mini: Smart Split Bill CLI (Bagi Rata)

Dokumen ini berisi bedah kode lengkap baris demi baris (*line-by-line breakdown*) dari file [`bagi_rata.c`](file:///d:/project/splitt-bill/bagi_rata.c). Penjelasan disusun secara terstruktur dengan bahasa yang lugas, mudah dipahami mahasiswa teknik, dan langsung mengulas fungsi sintaksis serta logika pemrogramannya.

---

## Daftar Isi
1. [Struktur Pustaka & Header (Baris 1 - 3)](#1-struktur-pustaka--header-baris-1---3)
2. [Konstanta & Struktur Data (Baris 5 - 12)](#2-konstanta--struktur-data-baris-5---12)
3. [Fungsi Pembantu 1: format_rupiah() (Baris 14 - 28)](#3-fungsi-pembantu-1-format_rupiah-baris-14---28)
4. [Fungsi Pembantu 2: input_angka() (Baris 30 - 48)](#4-fungsi-pembantu-2-input_angka-baris-30---48)
5. [Fungsi Utama: main() & Variabel (Baris 50 - 66)](#5-fungsi-utama-main--variabel-baris-50---66)
6. [Validasi Jumlah Orang (Baris 67 - 78)](#6-validasi-jumlah-orang-baris-67---78)
7. [Input Data Peserta & Subtotal (Baris 80 - 89)](#7-input-data-peserta--subtotal-baris-80---89)
8. [Input Pajak, Servis & Kalkulasi Matematika (Baris 91 - 108)](#8-input-pajak-servis--kalkulasi-matematika-baris-91---108)
9. [Format & Cetak Tabel ASCII 77 Kolom (Baris 109 - 143)](#9-format--cetak-tabel-ascii-77-kolom-baris-109---143)
10. [Siklus Perulangan Sesi & Penutup (Baris 144 - 158)](#10-siklus-perulangan-sesi--penutup-baris-144---158)

---

### 1. Struktur Pustaka & Header (Baris 1 - 3)

```c
1: #include <stdio.h>
2: #include <string.h>
3: #include <math.h>
```

* **Baris 1 (`#include <stdio.h>`):** 
  Mengimpor pustaka *Standard Input Output*. Dibutuhkan untuk fungsi membaca input (`fgets`, `sscanf`) dan mencetak teks ke layar terminal (`printf`, `snprintf`).
* **Baris 2 (`#include <string.h>`):** 
  Mengimpor pustaka pemrosesan teks (*string*). Digunakan untuk menghitung panjang teks (`strlen`), menyalin teks (`strcpy`), dan memotong karakter enter/newline (`strcspn`).
* **Baris 3 (`#include <math.h>`):** 
  Mengimpor pustaka fungsi matematika C. Digunakan untuk fungsi `ceil()` (membulatkan pecahan ke atas) dan `round()` (membulatkan ke bilangan bulat terdekat).

---

### 2. Konstanta & Struktur Data (Baris 5 - 12)

```c
5: #define MAX_PESERTA 50
6: 
7: struct Peserta {
8:     char nama[50];
9:     double subtotal;
10:    double tagihan_kotor;
11:    long tagihan_akhir;
12: };
```

* **Baris 5 (`#define MAX_PESERTA 50`):** 
  Mendefinisikan nilai konstan maksimum peserta dalam satu sesi adalah 50 orang. Jika di masa depan ingin diubah (misal 100), cukup ganti angka di baris ini saja.
* **Baris 7 - 12 (`struct Peserta`):** 
  Tipe data bentukan (*record*) untuk membungkus seluruh atribut satu orang peserta ke dalam satu wadah:
  * `char nama[50]`: Menampung nama peserta hingga 49 karakter (+ 1 karakter *null-terminator* `\0`).
  * `double subtotal`: Nilai rupiah pesanan awal peserta sebelum pajak/servis (bilangan desimal).
  * `double tagihan_kotor`: Nilai tagihan riil setelah dikalikan persentase pajak dan servis (masih ada angka pecahan/sen).
  * `long tagihan_akhir`: Nilai final yang wajib ditransfer setelah dibulatkan ke atas ke kelipatan Rp1.000 (bilangan bulat panjang).

---

### 3. Fungsi Pembantu 1: format_rupiah() (Baris 14 - 28)

Tujuan fungsi ini adalah mengubah bilangan bulat seperti `46000` menjadi format string Rupiah dengan tanda titik pemisah ribuan: `"Rp 46.000"`.

```c
14: void format_rupiah(long nilai, char *hasil) {
15:     char temp[32];
16:     snprintf(temp, sizeof(temp), "%ld", nilai);
17:     int len = (int)strlen(temp);
18: 
19:     strcpy(hasil, "Rp ");
20:     int idx = 3;
21:     for (int i = 0; i < len; i++) {
22:         hasil[idx++] = temp[i];
23:         if ((len - 1 - i) > 0 && (len - 1 - i) % 3 == 0) {
24:             hasil[idx++] = '.';
25:         }
26:     }
27:     hasil[idx] = '\0';
28: }
```

* **Baris 14:** Menerima parameter angka numerik `nilai` dan pointer buffer string `hasil` untuk menampung teks jadinya.
* **Baris 15 - 16:** `snprintf` mengonversi angka `nilai` menjadi string biasa di dalam array `temp` secara aman dari potensi buffer overflow.
* **Baris 17:** Menghitung jumlah digit angka yang tersimpan di `temp` menggunakan `strlen`.
* **Baris 19 - 20:** Mengisi awal string `hasil` dengan teks `"Rp "` (panjang 3 karakter), sehingga indeks tulis berikutnya dimulai dari indeks ke-3.
* **Baris 21 - 26 (`for loop`):**
  * `hasil[idx++] = temp[i]`: Menyalin satu per satu digit angka dari `temp` ke `hasil`.
  * `(len - 1 - i)`: Menghitung sisa berapa digit lagi di sebelah kanan.
  * `if ((len - 1 - i) > 0 && (len - 1 - i) % 3 == 0)`: Logika inti matematika ribuan. Jika sisa digit di kanan merupakan kelipatan 3 (dan bukan digit terakhir), maka selipkan karakter titik (`.`). Contoh angka `46000` (panjang 5): saat di digit `6` (sisa 3 digit di kanan), titik disisipkan menjadi `Rp 46.`.
* **Baris 27:** Menambahkan karakter penutup string `\0` (*null character*) di akhir buffer.

---

### 4. Fungsi Pembantu 2: input_angka() (Baris 30 - 48)

Tujuan fungsi ini adalah membaca input angka desimal dari terminal secara aman. Jika user tidak sengaja mengetik huruf (misal `"akram"`), program tidak akan error atau loop terus-menerus, melainkan menampilkan peringatan dan meminta input ulang.

```c
30: double input_angka(const char *pesan) {
31:     char buffer[100];
32:     double nilai;
33:     char sisa;
34: 
35:     while (1) {
36:         printf("%s", pesan);
37:         if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
38:             continue;
39:         }
40: 
41:         /* validasi hanya angka desimal positif, tolak huruf atau karakter acak */
42:         if (sscanf(buffer, "%lf %c", &nilai, &sisa) == 1 && nilai >= 0.0) {
43:             return nilai;
44:         }
45: 
46:         printf("tolong masukin angka ya\n");
47:     }
48: }
```

* **Baris 30:** Fungsi menerima teks prompt `pesan` (misal `"Subtotal pesanan: "`) dan mengembalikan nilai angka `double`.
* **Baris 35 (`while (1)`):** Perulangan tanpa henti (*infinite loop*) yang hanya akan berhenti jika pengguna memberikan input yang valid.
* **Baris 36 - 39:** 
  * `printf("%s", pesan)`: Menampilkan label input.
  * `fgets(buffer, sizeof(buffer), stdin)`: Membaca satu baris penuh dari keyboard ke dalam `buffer`. Keunggulan `fgets` dibanding `scanf` adalah tidak menyisakan karakter sampah di *stream stdin*.
* **Baris 42:** `sscanf(buffer, "%lf %c", &nilai, &sisa) == 1 && nilai >= 0.0`
  * Mencoba membaca angka desimal (`%lf`).
  * Format `%c` berguna untuk menangkap jika ada karakter asing setelah angka (misal user mengetik `"20000abc"`).
  * Jika hanya angka murni yang dimasukkan, `sscanf` akan mengembalikan nilai tepat `1`.
  * Syarat `nilai >= 0.0` memastikan nominal tidak boleh minus/negatif.
* **Baris 43:** Jika input lolos validasi, kembalikan `nilai` dan keluar dari fungsi.
* **Baris 46:** Jika input bukan angka (misal huruf), cetak alert teks `"tolong masukin angka ya"` dan loop akan otomatis mengulang prompt.

---

### 5. Fungsi Utama: main() & Variabel (Baris 50 - 66)

```c
50: int main(void) {
51:     char ulang;
52: 
53:     do {
54:         int jumlah_orang;
55:         struct Peserta peserta[MAX_PESERTA];
56:         double pajak, service;
57:         double total_makanan = 0.0;
58:         double faktor_biaya;
59:         double total_asli;
60:         long total_terkumpul = 0;
61:         long sisa_tip;
62: 
63:         printf("=============================================================================\n");
64:         printf("                           BAGI RATA - SPLIT BILL                            \n");
65:         printf("=============================================================================\n\n");
```

* **Baris 50:** Titik awal eksekusi program C (`main`).
* **Baris 51:** Variabel `ulang` bertipe `char` untuk menyimpan respons user `'y'` atau `'n'`.
* **Baris 53 (`do {`):** Membuka blok perulangan sesi. Memastikan seluruh alur program berjalan minimal 1 kali.
* **Baris 54 - 61:** Deklarasi variabel lokal tiap sesi:
  * `jumlah_orang`: Jumlah peserta yang ikut patungan ($N$).
  * `peserta[MAX_PESERTA]`: Array dari struct untuk menyimpan data hingga 50 orang.
  * `pajak`, `service`: Persentase biaya restoran dalam bentuk persen.
  * `total_makanan`: Akumulasi harga seluruh makanan yang dipesan.
  * `faktor_biaya`: Faktor pengali gabungan ($1 + \text{persen}/100$).
  * `total_asli`: Tagihan riil struk restoran (total makanan dikali faktor biaya).
  * `total_terkumpul`: Total uang rupiah yang terkumpul dari seluruh peserta setelah pembulatan ke atas.
  * `sisa_tip`: Selisih dana lebihan hasil pembulatan untuk uang tip pelayan atau kas.
* **Baris 63 - 65:** Mencetak banner judul dengan panjang tepat 77 karakter agar serasi dengan lebar tabel di bawah.

---

### 6. Validasi Jumlah Orang (Baris 67 - 78)

```c
67:         while (1) {
68:             char buf_orang[100];
69:             char sisa_orang;
70:             printf("Masukkan jumlah orang (maks 50): ");
71:             if (fgets(buf_orang, sizeof(buf_orang), stdin) != NULL) {
72:                 if (sscanf(buf_orang, "%d %c", &jumlah_orang, &sisa_orang) == 1 &&
73:                     jumlah_orang > 0 && jumlah_orang <= MAX_PESERTA) {
74:                     break;
75:                 }
76:             }
77:             printf("tolong masukin angka ya\n");
78:         }
```

* **Baris 67 - 78:** Menerapkan logika validasi aman untuk input bilangan bulat (`int`):
  * Membaca baris ke `buf_orang` dengan `fgets`.
  * Memeriksa dengan `sscanf`: harus berupa angka bulat positif (`%d`), tidak boleh ada huruf di belakangnya, bernilai $> 0$, dan tidak melebihi `MAX_PESERTA` (50).
  * Jika valid, eksekusi `break` keluar dari loop. Jika salah, cetak alert `"tolong masukin angka ya"`.

---

### 7. Input Data Peserta & Subtotal (Baris 80 - 89)

```c
80:         for (int i = 0; i < jumlah_orang; i++) {
81:             printf("\nData orang ke-%d:\n", i + 1);
82:             printf("Nama            : ");
83:             if (fgets(peserta[i].nama, sizeof(peserta[i].nama), stdin) != NULL) {
84:                 peserta[i].nama[strcspn(peserta[i].nama, "\r\n")] = '\0';
85:             }
86: 
87:             peserta[i].subtotal = input_angka("Subtotal pesanan: ");
88:             total_makanan += peserta[i].subtotal;
89:         }
```

* **Baris 80:** Perulangan `for` sebanyak `jumlah_orang` kali untuk mencatat data masing-masing peserta.
* **Baris 83 - 85:** 
  * `fgets(peserta[i].nama, ...)` membaca nama peserta. Kelebihannya: mendukung nama lengkap yang mengandung spasi (contoh: `"Budi Santoso"`).
  * `strcspn(peserta[i].nama, "\r\n")`: Mencari letak karakter tombol Enter (*newline*), lalu menggantinya dengan `\0` agar string bersih tanpa baris baru.
* **Baris 87:** Memanggil fungsi `input_angka()` untuk membaca harga makanan individu dengan proteksi penolakan huruf.
* **Baris 88:** Menambahkan subtotal orang ke-$i$ ke dalam akumulator `total_makanan`.

---

### 8. Input Pajak, Servis & Kalkulasi Matematika (Baris 91 - 108)

```c
91:         pajak = input_angka("\nPersentase pajak (%)   : ");
92:         service = input_angka("Persentase service (%) : ");
93: 
94:         /* faktor pengali gabungan biar pajak & servis dibagi proporsional */
95:         faktor_biaya = 1.0 + ((pajak + service) / 100.0);
96: 
97:         for (int i = 0; i < jumlah_orang; i++) {
98:             peserta[i].tagihan_kotor = peserta[i].subtotal * faktor_biaya;
99: 
100:            /* bulatin ke atas kelipatan 1000 pake ceil, kurangi dikit biar presisi float aman */
101:            peserta[i].tagihan_akhir = (long)(ceil((peserta[i].tagihan_kotor - 1e-4) / 1000.0) * 1000.0);
102: 
103:            total_terkumpul += peserta[i].tagihan_akhir;
104:        }
105: 
106:        total_asli = total_makanan * faktor_biaya;
107:        sisa_tip = total_terkumpul - (long)round(total_asli);
```

* **Baris 91 - 92:** Membaca persentase pajak dan servis (misal `10` dan `2`) menggunakan `input_angka()`.
* **Baris 94 - 95:** Menghitung faktor pengali proporsional:
  $$\text{faktor\_biaya} = 1.0 + \frac{10 + 2}{100} = 1.12$$
* **Baris 97 - 104 (`for loop` kalkulasi individu):**
  * **Baris 98:** Menghitung tagihan kotor: `peserta[i].subtotal * faktor_biaya`.
  * **Baris 101 (Koreksi Presisi Floating-Point IEEE 754):**
    ```c
    (long)(ceil((peserta[i].tagihan_kotor - 1e-4) / 1000.0) * 1000.0)
    ```
    * Di bahasa C, perkalian pecahan seperti `50000 * 1.10` dalam representasi memori biner menghasilkan `55000.000000000007`. Jika langsung di-`ceil() / 1000`, nilainya akan melompat keliru menjadi `56000`.
    * Pengurangan toleransi epsilon kecil (`- 1e-4` atau `0.0001`) menetralkan galat biner tersebut sehingga angka pas tetap bulat pas (`55000`), sedangkan angka ganjil seperti `28750` tetap dibulatkan ke atas menjadi `29000`.
  * **Baris 103:** Menjumlahkan tagihan bulat ke akumulator `total_terkumpul`.
* **Baris 106:** Menghitung tagihan riil struk restoran tanpa pembulatan (`total_makanan * faktor_biaya`).
* **Baris 107:** Menghitung uang kembalian/tip kasir: `total_terkumpul - total_asli`.

---

### 9. Format & Cetak Tabel ASCII 77 Kolom (Baris 109 - 143)

Bagian ini mencetak laporan transaksi dalam bentuk tabel yang presisi dan estetis.

```c
109:        printf("\n+---------------------------------------------------------------------------+\n");
110:        printf("|                             RINGKASAN TAGIHAN                             |\n");
111:        printf("+-----+------------------+----------------+----------------+----------------+\n");
112:        printf("| %-3s | %-16s | %14s | %14s | %14s |\n",
113:               "No", "Nama", "Pesanan", "Tagihan Asli", "Wajib Bayar");
114:        printf("+-----+------------------+----------------+----------------+----------------+\n");
```

* **Baris 109 - 114:** 
  * Mencetak kotak pembatas dan header kolom.
  * Lebar total baris adalah tepat **77 karakter** (`1 char kiri + 75 char isi + 1 char kanan`).
  * Penentu format:
    * `%-3s`: Nomor urut rata kiri (lebar 3).
    * `%-16s`: Nama peserta rata kiri (lebar 16).
    * `%14s`: Kolom nominal rata kanan (lebar 14) agar lurus dengan angka uang.

```c
116:        char teks_pesanan[32], teks_asli[32], teks_bayar[32];
117:        for (int i = 0; i < jumlah_orang; i++) {
118:            format_rupiah((long)round(peserta[i].subtotal), teks_pesanan);
119:            format_rupiah((long)round(peserta[i].tagihan_kotor), teks_asli);
120:            format_rupiah(peserta[i].tagihan_akhir, teks_bayar);
121: 
122:            printf("| %-3d | %-16s | %14s | %14s | %14s |\n",
123:                   i + 1, peserta[i].nama, teks_pesanan, teks_asli, teks_bayar);
124:        }
```

* **Baris 116 - 124:** 
  * Mengonversi tiap nominal ke format Rupiah bertitik menggunakan `format_rupiah()`.
  * Mencetak baris peserta dengan `%14s` sehingga seluruh simbol `Rp` dan angka tersusun rapi rata kanan.

```c
126:        printf("+-----+------------------+----------------+----------------+----------------+\n");
127: 
128:        char teks_tot_makan[32], teks_tot_asli[32], teks_tot_kumpul[32], teks_tip[32];
129:        char teks_pajak_info[64];
130: 
131:        format_rupiah((long)round(total_makanan), teks_tot_makan);
132:        format_rupiah((long)round(total_asli), teks_tot_asli);
133:        format_rupiah(total_terkumpul, teks_tot_kumpul);
134:        format_rupiah(sisa_tip, teks_tip);
135:        snprintf(teks_pajak_info, sizeof(teks_pajak_info), "%.1f%% (Faktor: %.4f)", pajak + service, faktor_biaya);
136: 
137:        printf("| Total Pesanan Makanan  : %-48s |\n", teks_tot_makan);
138:        printf("| Pajak & Service        : %-48s |\n", teks_pajak_info);
139:        printf("| Total Riil Struk       : %-48s |\n", teks_tot_asli);
140:        printf("| Total Uang Terkumpul   : %-48s |\n", teks_tot_kumpul);
141:        printf("| Sisa Lebih / Tip Kasir : %-48s |\n", teks_tip);
142:        printf("+---------------------------------------------------------------------------+\n");
```

* **Baris 126 - 142:**
  * Mencetak blok ringkasan keuangan.
  * Teks label sebelum titik dua berukuran persis 25 karakter.
  * Penentu format `%-48s` mengisi sisa 48 karakter di sebelah kanan, sehingga:
    $$2 \text{ (batas kiri)} + 25 \text{ (label)} + 48 \text{ (nilai)} + 2 \text{ (batas kanan)} = 77 \text{ karakter}$$
  * Hal ini membuat garis vertikal penutup sebelah kanan (`|`) lurus vertikal sempurna tanpa bergeser 1 spasi pun.

---

### 10. Siklus Perulangan Sesi & Penutup (Baris 144 - 158)

```c
144:        printf("\nHitung tagihan baru lagi? (y/n): ");
145:        char buf_ulang[50];
146:        if (fgets(buf_ulang, sizeof(buf_ulang), stdin) != NULL) {
147:            ulang = buf_ulang[0];
148:        } else {
149:            ulang = 'n';
150:        }
151:        printf("\n");
152: 
153:    } while (ulang == 'y' || ulang == 'Y');
154: 
155:    printf("Terima kasih telah menggunakan Bagi Rata!\n");
156:    return 0;
157: }
```

* **Baris 144 - 150:**
  * Menanyakan apakah pengguna ingin melakukan perhitungan baru.
  * Membaca respon menggunakan `fgets` dan mengambil karakter pertama `buf_ulang[0]`. Pendekatan ini aman dan tidak meninggalkan karakter enter liar di memori.
* **Baris 153 (`} while (ulang == 'y' || ulang == 'Y');`):**
  * Jika user memasukkan huruf `'y'` atau `'Y'`, program akan kembali ke Baris 53 (`do`) untuk memulai transaksi baru.
  * Jika user memasukkan `'n'` atau huruf lain, loop berhenti.
* **Baris 155 - 156:** 
  * Mencetak salam penutup.
  * `return 0;`: Mengembalikan status keberhasilan eksekusi (kode keluar 0) kepada sistem operasi Windows.

---

## Ringkasan Konsep Kunci Bahasa C yang Digunakan

| Konsep C | Baris Kode | Manfaat & Alasan Pemilihan |
| :--- | :--- | :--- |
| **`struct`** | Baris 7 - 12 | Mengelompokkan data nama, subtotal, dan tagihan dalam satu entitas yang rapi. |
| **`fgets` + `sscanf`** | Baris 37, 42, 71 | Menggantikan kelemahan `scanf` mentah; kebal terhadap input huruf yang menyebabkan crash. |
| **`snprintf`** | Baris 16, 135 | Penggabungan string yang aman dengan batas kapasitas memori buffer. |
| **`ceil()` + Epsilon** | Baris 101 | Pembulatan ke atas kelipatan 1.000 dengan proteksi presisi pecahan biner IEEE 754. |
| **Format Width (`%14s`, `%-48s`)** | Baris 112, 122, 137 | Mengunci tata letak visual terminal konsol agar tabel sejajar simetris tepat 77 kolom. |
| **`do-while`** | Baris 53, 153 | Memastikan CLI berjalan minimal 1 kali dan menjaga terminal tidak tertutup sebelum izin user. |
