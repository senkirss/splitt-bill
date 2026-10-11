# Product Requirement Document (PRD)
## Bagi Rata - Smart Split Bill CLI

---

## 1. Informasi Proyek

* **Nama Proyek:** Bagi Rata
* **Tipe Aplikasi:** Console / Command Line Interface (CLI)
* **Bahasa Pemrograman:** C (Standar C99 / C11)
* **Status:** Selesai Diimplementasi & Terverifikasi (Proyek Mini Mahasiswa)

---

## 2. Pendahuluan

Aktivitas makan dan berkumpul bersama rekan sejawat merupakan kegiatan sosial yang rutin dilakukan oleh mahasiswa maupun pekerja muda. Namun, fase penyelesaian transaksi di akhir acara sering kali menimbulkan ketidaknyamanan, ketidakjelasan nominal, dan memakan waktu cukup lama karena perhitungan pembagian biaya yang rumit. 

Proyek **Bagi Rata** menghadirkan solusi perangkat lunak terminal yang ringkas, transparan, berkeadilan, dan tahan kesalahan (*fault-tolerant*). Program ini memanfaatkan kapabilitas komputasi aritmatika dasar, pemrosesan larik (*array*) dan *struct*, validasi *stream buffer* (`fgets` + `sscanf`), serta fungsi pembulatan matematika bawaan bahasa C (`ceil()`) tanpa dependensi pustaka luar yang rumit.

---

## 3. Pernyataan Masalah

Beberapa kendala utama yang kerap dihadapi saat melakukan pembagian tagihan makan bersama meliputi:

1. **Ketidakadilan Metode *Split Equal*:** Membagi total tagihan akhir sama rata merugikan individu yang memesan menu dengan harga lebih murah atau hanya memesan sedikit makanan.
2. **Kompleksitas Perhitungan Pajak & Biaya Tambahan:** Pajak restoran (PB1) dan biaya layanan dihitung dari total akumulasi, sehingga membaginya secara proporsional ke tiap menu secara manual melalui kalkulator biasa sangat merepotkan.
3. **Friksi Nominal Transfer (Nominal Ganjil/Receh):** Hasil pembagian manual persentase sering kali menghasilkan nominal ganjil (misalnya Rp34.237). Hal ini menyulitkan saat transfer menggunakan mobile banking atau dompet digital karena nominal pecahan tidak efisien.
4. **Ketidakjelasan Alokasi Sisa Pembulatan:** Jika ada inisiatif pembulatan nominal, kelebihan dana sering kali tidak tercatat secara transparan apakah dialokasikan untuk uang tip pelayan atau kas kelompok.
5. **Kerapuhan Input CLI Tradisional:** Penggunaan `scanf` standar di bahasa C rentan terhadap *infinite loop* atau *crashing* jika pengguna tidak sengaja memasukkan teks/huruf saat program meminta angka numerik.
6. **Program Menutup Otomatis:** Program CLI semester awal sering kali langsung menutup jendela terminal setelah perhitungan selesai tanpa memberi kesempatan pengguna melihat hasil atau menghitung transaksi lain.

---

## 4. Pemecahan Masalah & Proposisi Nilai

Bagi Rata menawarkan pendekatan komputasi terstruktur melalui fitur-fitur berikut:

* **Distribusi Biaya Proporsional (*Proportional Cost Factor*):** Menggunakan faktor pengali gabungan sehingga beban pajak dan servis ditanggung secara adil sesuai nilai konsumsi masing-masing orang.
* **Pembulatan Otomatis (*Smart Ceiling Rounding*):** Membulatkan setiap tagihan individu ke atas ke kelipatan seribu terdekat ($\text{Rp}1.000$) menggunakan fungsi `ceil()` untuk menghilangkan pecahan ganjil dan mempermudah transaksi digital.
* **Koreksi Presisi Floating-Point IEEE 754:** Mengantisipasi ketidakakuratan representasi biner bilangan pecahan di C (toleransi $\epsilon = 10^{-4}$) agar nilai kelipatan bulat pas tidak melompat ke atas.
* **Transparansi Rekonsiliasi Kas (*Change Transparency*):** Menghitung dan menampilkan total akumulasi dana lebih hasil pembulatan untuk digunakan sebagai tip kasir atau kas bersama secara terbuka.
* **Validasi Input Kebal Error (*Fault-Tolerant Input*):** Menolak karakter selain angka pada subtotal, pajak, dan servis secara otomatis menggunakan kombinasi `fgets` dan `sscanf`, menampilkan peringatan `"tolong masukin angka ya"`, dan mengulang permintaan input tanpa merusak buffer memori.
* **Penyajian Tabel Presisi & Terformat (*Symmetric ASCII Box*):** Mengonversi angka ke format Rupiah standar (`Rp XX.XXX`) dengan pemisah ribuan dan menyajikan tabel simetris selebar 77 kolom yang sejajar rapi.
* **Perulangan Sesi Transaksi (*Interactive Session Loop*):** Membungkus eksekusi dalam perulangan `do-while` sehingga pengguna dapat menghitung transaksi baru (`[y/n]`) tanpa membuka ulang *executable*.

---

## 5. Ruang Lingkup & Kebutuhan Fungsional

### 5.1 Fitur Inti (*Core Features*)
1. **Pencatatan Peserta & Pesanan:**
   * Menerima input jumlah orang ($N$, maksimal 50 orang).
   * Menerima input nama peserta (mendukung karakter spasi hingga 49 karakter).
   * Menerima input total nilai pesanan individu (validasi angka non-negatif).
2. **Pengaturan Pajak & Servis:**
   * Menerima input persentase pajak restoran (misal: $10\%$).
   * Menerima input persentase biaya servis (misal: $5\%$).
   * Menolak input huruf atau angka negatif dengan alert `"tolong masukin angka ya"`.
3. **Kalkulasi Biaya Terpadu:**
   * Menghitung total kotor seluruh makanan.
   * Menghitung faktor pengali biaya tambahan: $\text{faktor} = 1 + \frac{\text{pajak} + \text{servis}}{100}$.
   * Menghitung tagihan riil dan tagihan setelah pembulatan ke kelipatan Rp1.000.
4. **Laporan & Rekonsiliasi Kas:**
   * Menampilkan tabel berbingkai rapi (lebar 77 kolom).
   * Menampilkan kolom: No, Nama, Pesanan, Tagihan Asli, dan Wajib Bayar (rata kanan).
   * Menampilkan ringkasan: Total Makanan, Pajak & Service (Faktor), Total Riil Struk, Total Terkumpul, serta Sisa Lebih / Tip Kasir.
5. **Perulangan Sesi:**
   * Menanyakan konfirmasi `Hitung tagihan baru lagi? (y/n): ` di akhir proses.

### 5.2 Batasan Sistem (*Constraints*)
* Standar C99 murni tanpa pustaka eksternal pihak ketiga.
* Hanya menggunakan pustaka standar: `<stdio.h>`, `<string.h>`, `<math.h>`.
* Struktur kode prosedural dan modular santai gaya mahasiswa (maksimal 1-2 fungsi bantuan sederhana: `format_rupiah` dan `input_angka`).
* Penyimpanan data bersifat sementara (*in-memory*) selama eksekusi berjalan.

---

## 6. Kebutuhan Teknis & Spesifikasi Algoritma

### 6.1 Pustaka Standar
* `<stdio.h>`: Operasi I/O konsol (`printf`, `fgets`, `sscanf`, `snprintf`).
* `<string.h>`: Operasi manipulasi string (`strlen`, `strcpy`, `strcspn`).
* `<math.h>`: Fungsi matematika pembulatan (`ceil`, `round`).

### 6.2 Rumus dan Formulasi Matematis

1. **Total Konsumsi Makanan:**
   $$\text{total\_makanan} = \sum_{i=0}^{N-1} \text{subtotal}[i]$$

2. **Faktor Biaya Tambahan (*Cost Multiplier*):**
   $$\text{faktor\_biaya} = 1 + \left( \frac{\text{pajak\_persen} + \text{service\_persen}}{100} \right)$$

3. **Tagihan Kotor Individu:**
   $$\text{tagihan\_kotor}[i] = \text{subtotal}[i] \times \text{faktor\_biaya}$$

4. **Tagihan Final (Pembulatan ke Kelipatan Rp1.000 dengan Koreksi Epsilon):**
   $$\text{tagihan\_akhir}[i] = \left\lfloor \frac{\text{tagihan\_kotor}[i] - 10^{-4}}{1000} + 1 \right\rfloor \times 1000 = \text{ceil}\left(\frac{\text{tagihan\_kotor}[i] - 10^{-4}}{1000}\right) \times 1000$$

5. **Selisih Akumulasi Pembulatan (Kas Sisa / Tip):**
   $$\text{total\_asli} = \text{total\_makanan} \times \text{faktor\_biaya}$$
   $$\text{total\_terkumpul} = \sum_{i=0}^{N-1} \text{tagihan\_akhir}[i]$$
   $$\text{sisa\_tip} = \text{total\_terkumpul} - \text{round}(\text{total\_asli})$$

---

## 7. Alur Kerja Sistem (*Updated Workflow*)

```
[Mulai Eksekusi main()]
   │
   ▼
[DO: Mulai Sesi Transaksi]
   │
   ▼
[Input Jumlah Orang: N] ──(Bukan Angka / N <= 0 / N > 50)──► [Cetak: "tolong masukin angka ya"] ──┐
   │                                                                                              │
   ◄──────────────────────────────────────────────────────────────────────────────────────────────┘
   │
   ▼
[Loop i = 0 s.d. N-1]
   ├─► [Input Nama Peserta (mendukung spasi)]
   └─► [Input Subtotal Pesanan] ──(Input Huruf/Negatif)──► [Cetak: "tolong masukin angka ya"] ──┐
          │                                                                                     │
          ◄─────────────────────────────────────────────────────────────────────────────────────┘
   │
   ▼
[Input Pajak & Service] ──(Input Huruf/Negatif)──► [Cetak: "tolong masukin angka ya"] ──┐
   │                                                                                    │
   ◄────────────────────────────────────────────────────────────────────────────────────┘
   │
   ▼
[Kalkulasi Matematis]
   ├─► faktor_biaya = 1.0 + ((pajak + service) / 100.0)
   ├─► Loop i: tagihan_kotor[i] & tagihan_akhir[i] via ceil() dengan toleransi float
   └─► total_asli, total_terkumpul, sisa_tip
   │
   ▼
[Cetak Tabel Ringkasan Presisi (Lebar 77 Kolom)]
   ├─► Header Box & Kolom Header Rata Kanan
   ├─► Baris Peserta Terformat Rupiah (Rp XX.XXX)
   └─► Ringkasan Finansial & Tip Kasir
   │
   ▼
[Prompt: Hitung tagihan baru lagi? (y/n)]
   │
   ├─► ['y' / 'Y'] ──► [Ulangi Siklus Sesi]
   │
   └─► ['n' / lainnya]
          │
          ▼
       [Cetak Pesan Penutup & Selesai]
```

---

## 8. Skenario Pengujian (*Test Cases*)

### 8.1 Skenario Standar (PRD Original)
* **Input:**
  * $N = 3$
  * Budi: $\text{Rp}40.000$, Ani: $\text{Rp}25.000$, Cici: $\text{Rp}15.000$
  * Pajak: $10\%$, Service: $5\%$
* **Hasil:**
  * Budi: $\text{Rp}46.000$ (kotor: $46.000$)
  * Ani: $\text{Rp}29.000$ (kotor: $28.750$)
  * Cici: $\text{Rp}18.000$ (kotor: $17.250$)
  * Total Struk: $\text{Rp}92.000$, Terkumpul: $\text{Rp}93.000$, Tip: $\text{Rp}1.000$.

### 8.2 Skenario Pengujian Lanjutan (5 Peserta & Kasus Pajak 12%)
* **Input:**
  * $N = 5$
  * Akram: $\text{Rp}10.000$, Puput: $\text{Rp}20.000$, Ayah: $\text{Rp}30.000$, Biyu: $\text{Rp}40.000$, Bunda: $\text{Rp}50.000$
  * Pajak: $10\%$, Service: $2\%$ (Total 12%)
* **Hasil:**
  * Akram: $\text{Rp}12.000$, Puput: $\text{Rp}23.000$, Ayah: $\text{Rp}34.000$, Biyu: $\text{Rp}45.000$, Bunda: $\text{Rp}56.000$
  * Total Makanan: $\text{Rp}150.000$, Total Riil: $\text{Rp}168.000$, Terkumpul: $\text{Rp}170.000$, Tip: $\text{Rp}2.000$.

### 8.3 Skenario Penanganan Kesalahan Input (*Error Recovery*)
* **Input:**
  * Subtotal diisi huruf `"akram"` $\rightarrow$ Sistem mencetak `"tolong masukin angka ya"`, buffer dibersihkan, dan meminta input ulang.
  * Pajak diisi huruf `"abc"` $\rightarrow$ Sistem mencetak `"tolong masukin angka ya"` dan meminta input ulang.
  * Program tidak mengalami *crash* atau *infinite loop*.

---

## 9. Struktur Kode Terpasang

Implementasi final berada pada [bagi_rata.c](file:///d:/project/splitt-bill/bagi_rata.c) dengan struktur:
1. `struct Peserta`: Menyimpan nama, subtotal, tagihan kotor, dan tagihan akhir.
2. `void format_rupiah(long nilai, char *hasil)`: Menghasilkan format string `Rp XX.XXX`.
3. `double input_angka(const char *pesan)`: Mengisolasi pembacaan angka via `fgets` + `sscanf`.
4. `main()`: Mengelola siklus interaktif `do-while`, perulangan data `for`, kalkulasi matematika, dan pencetakan tabel 77 karakter.