#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX_PESERTA 50

struct Peserta {
    char nama[50];
    double subtotal;
    double tagihan_kotor;
    long tagihan_akhir;
};

void format_rupiah(long nilai, char *hasil) {
    char temp[32];
    snprintf(temp, sizeof(temp), "%ld", nilai);
    int len = (int)strlen(temp);

    strcpy(hasil, "Rp ");
    int idx = 3;
    for (int i = 0; i < len; i++) {
        hasil[idx++] = temp[i];
        if ((len - 1 - i) > 0 && (len - 1 - i) % 3 == 0) {
            hasil[idx++] = '.';
        }
    }
    hasil[idx] = '\0';
}

double input_angka(const char *pesan) {
    char buffer[100];
    double nilai;
    char sisa;

    while (1) {
        printf("%s", pesan);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }

        if (sscanf(buffer, "%lf %c", &nilai, &sisa) == 1 && nilai >= 0.0) {
            return nilai;
        }

        printf("tolong masukin angka ya\n");
    }
}

int main(void) {
    char ulang;

    do {
        int jumlah_orang;
        struct Peserta peserta[MAX_PESERTA];
        double pajak, service;
        double total_makanan = 0.0;
        double faktor_biaya;
        double total_asli;
        long total_terkumpul = 0;
        long sisa_tip;

        printf("=============================================================================\n");
        printf("                           BAGI RATA - SPLIT BILL                            \n");
        printf("=============================================================================\n\n");

        while (1) {
            char buf_orang[100];
            char sisa_orang;
            printf("Masukkan jumlah orang (maks 50): ");
            if (fgets(buf_orang, sizeof(buf_orang), stdin) != NULL) {
                if (sscanf(buf_orang, "%d %c", &jumlah_orang, &sisa_orang) == 1 &&
                    jumlah_orang > 0 && jumlah_orang <= MAX_PESERTA) {
                    break;
                }
            }
            printf("tolong masukin angka ya\n");
        }

        for (int i = 0; i < jumlah_orang; i++) {
            printf("\nData orang ke-%d:\n", i + 1);
            printf("Nama            : ");
            if (fgets(peserta[i].nama, sizeof(peserta[i].nama), stdin) != NULL) {
                peserta[i].nama[strcspn(peserta[i].nama, "\r\n")] = '\0';
            }

            peserta[i].subtotal = input_angka("Subtotal pesanan: ");
            total_makanan += peserta[i].subtotal;
        }

        pajak = input_angka("\nPersentase pajak (%)   : ");
        service = input_angka("Persentase service (%) : ");

        /* faktor pengali gabungan biar pajak & servis dibagi proporsional */
        faktor_biaya = 1.0 + ((pajak + service) / 100.0);

        for (int i = 0; i < jumlah_orang; i++) {
            peserta[i].tagihan_kotor = peserta[i].subtotal * faktor_biaya;

            /* bulatin ke atas kelipatan 1000 pake ceil, kurangi dikit biar presisi float aman */
            peserta[i].tagihan_akhir = (long)(ceil((peserta[i].tagihan_kotor - 1e-4) / 1000.0) * 1000.0);

            total_terkumpul += peserta[i].tagihan_akhir;
        }

        total_asli = total_makanan * faktor_biaya;
        sisa_tip = total_terkumpul - (long)round(total_asli);

        printf("\n+---------------------------------------------------------------------------+\n");
        printf("|                             RINGKASAN TAGIHAN                             |\n");
        printf("+-----+------------------+----------------+----------------+----------------+\n");
        printf("| %-3s | %-16s | %14s | %14s | %14s |\n",
               "No", "Nama", "Pesanan", "Tagihan Asli", "Wajib Bayar");
        printf("+-----+------------------+----------------+----------------+----------------+\n");

        char teks_pesanan[32], teks_asli[32], teks_bayar[32];
        for (int i = 0; i < jumlah_orang; i++) {
            format_rupiah((long)round(peserta[i].subtotal), teks_pesanan);
            format_rupiah((long)round(peserta[i].tagihan_kotor), teks_asli);
            format_rupiah(peserta[i].tagihan_akhir, teks_bayar);

            printf("| %-3d | %-16s | %14s | %14s | %14s |\n",
                   i + 1, peserta[i].nama, teks_pesanan, teks_asli, teks_bayar);
        }

        printf("+-----+------------------+----------------+----------------+----------------+\n");

        char teks_tot_makan[32], teks_tot_asli[32], teks_tot_kumpul[32], teks_tip[32];
        char teks_pajak_info[64];

        format_rupiah((long)round(total_makanan), teks_tot_makan);
        format_rupiah((long)round(total_asli), teks_tot_asli);
        format_rupiah(total_terkumpul, teks_tot_kumpul);
        format_rupiah(sisa_tip, teks_tip);
        snprintf(teks_pajak_info, sizeof(teks_pajak_info), "%.1f%% (Faktor: %.4f)", pajak + service, faktor_biaya);

        printf("| Total Pesanan Makanan  : %-48s |\n", teks_tot_makan);
        printf("| Pajak & Service        : %-48s |\n", teks_pajak_info);
        printf("| Total Riil Struk       : %-48s |\n", teks_tot_asli);
        printf("| Total Uang Terkumpul   : %-48s |\n", teks_tot_kumpul);
        printf("| Sisa Lebih / Tip Kasir : %-48s |\n", teks_tip);
        printf("+---------------------------------------------------------------------------+\n");

        printf("\nHitung tagihan baru lagi? (y/n): ");
        char buf_ulang[50];
        if (fgets(buf_ulang, sizeof(buf_ulang), stdin) != NULL) {
            ulang = buf_ulang[0];
        } else {
            ulang = 'n';
        }
        printf("\n");

    } while (ulang == 'y' || ulang == 'Y');

    printf("Terima kasih telah menggunakan Bagi Rata!\n");
    return 0;
}
