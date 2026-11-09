# The Sorcerer

Game arkade bertema Halloween bergaya *flappy bird*. Pemain mengendalikan seorang
penyihir yang harus terbang melewati gerombolan hantu, bertahan hidup, dan mengejar
skor tertinggi.

Dibuat dengan **C++** dan **SIGIL** (Sound, Input, and Graphics Integration Library).

---

## Fitur

### Gameplay
- Kontrol satu tombol: tekan `SPACE` untuk terbang.
- Tiga tipe hantu dengan perilaku berbeda: lambat, cepat, dan bergerak naik-turun.
- Sistem nyawa (5 nyawa) dengan *invulnerability* singkat setelah terkena serangan.
- Skor bertambah setiap kali hantu berhasil dilewati.
- Kesulitan alami dari gerakan dan variasi hantu.

### UI/UX
- **Layar awal** dengan judul, petunjuk kontrol, dan skor terbaik.
- **Pause** kapan saja tanpa kehilangan progres.
- **Game over** dengan ringkasan skor, skor terbaik, dan penanda `NEW BEST!`.
- **Highscore tersimpan** di `highscore.txt` dan dimuat otomatis saat game dibuka.
- **HUD** yang lebih jelas: ikon skor, label `SCORE`/`BEST`, dan bar nyawa tersegmentasi
  yang berubah warna (hijau → kuning → merah).
- **Feedback saat terkena serangan**: kilatan merah, getaran layar, dan penyihir berkedip
  selama masa kebal.
- **Animasi skor** saat skor bertambah.
- **Toggle musik** tanpa keluar dari game.
- **Debug overlay** tersembunyi, hanya muncul jika diaktifkan dengan `F3`.
- Latar dan awan tetap beranimasi di layar awal; pause membekukan permainan.

---

## Kontrol

| Tombol | Aksi |
| --- | --- |
| `SPACE` | Mulai game / terbang |
| `P` atau `ESC` | Pause / lanjutkan |
| `R` | Restart (saat pause atau game over) |
| `M` | Musik on/off |
| `F3` | Tampilkan/sembunyikan debug overlay |
| `ESC` | Keluar (hanya saat layar game over) |
| Tutup jendela | Keluar kapan saja |

---

## Cara Menjalankan

### Prasyarat
- Windows 10/11 (x64).
- Visual Studio dengan **Desktop development with C++**.
- Toolset `v145` sesuai konfigurasi project. Jika toolset lain yang terpasang,
  ubah `<PlatformToolset>` di `TheSorcerer.vcxproj` (mis. menjadi `v143`).
- **SIGIL** tersedia di `D:\libs\sigil` (berisi `include\sl.h`, `lib\sigil.lib`, dan DLL runtime).

### Build
1. Buka `Halloween.sln`.
2. Pilih konfigurasi **Release | x64** (atau Debug | x64).
3. Build project `TheSorcerer`.
4. Jalankan dari Visual Studio agar *working directory* berada di folder
   `TheSorcerer`, sehingga aset dapat ditemukan.

Jika ingin menjalankan `.exe` secara langsung, pastikan:
- file aset (`*.png`, `*.ttf`, `*.wav`, `*.WAV`) berada di *working directory* yang sama, dan
- DLL SIGIL (`sigil.dll`, `OpenAL32.dll`, `wrap_oal.dll`, `glew32.dll`) dapat ditemukan
  (letakkan di samping `.exe` atau tambahkan `D:\libs\sigil` ke `PATH`).

Project hanya mengonfigurasi include/library SIGIL untuk platform **x64**.
Konfigurasi Win32 belum memiliki path SIGIL.

---

## Struktur Project

| File | Fungsi |
| --- | --- |
| `main.cpp` | Loop utama, state game (`Ready`, `Playing`, `Paused`, `GameOver`), HUD, overlay, dan efek visual. |
| `sorcerer.h/.cpp` | Entitas penyihir: gravitasi, lompat, posisi, nyawa, dan ledakan partikel. |
| `ghost.h/.cpp` | Entitas hantu: tipe, kecepatan, gerakan sinus, dan status hidup. |
| `cloud.h/.cpp` | Awan parallax yang bergerak dan di-reset saat keluar layar. |
| `particle.h/.cpp` | Partikel ledakan saat penyihir terkena serangan. |
| `keylistener.h/.cpp` | Deteksi tombol *pressed* (edge) dan *held* (dengan delay). |
| `vec.h/.cpp` | Tipe vektor 2D sederhana untuk posisi dan kecepatan. |
| `image.h` | Deskriptor tekstur: `res_id`, `width`, `height`. |
| `README.md` | Dokumentasi project ini. |

### Aset
- `skynite.png` — latar malam.
- `cloud.png` — awan.
- `sorcerer.png` — karakter utama.
- `ghost.png` — hantu.
- `thunder.png` — ikon nyawa.
- `bigmoon.png` — aset bulan (tidak dipakai di build saat ini).
- `CFHalloween-Regular.ttf` — font alternatif (belum dipakai).
- `F25_Bank_Printer.ttf`, `Halloween Too.ttf` — font HUD dan judul.
- `halloween.WAV` — musik latar.
- `jump.wav`, `collision.wav` — efek suara.

---

## Alur Game

```
Ready ──SPACE──> Playing ──P/ESC──> Paused
                    │                 │
                    │                 └──P/ESC──> Playing
                    │
                    └──nyawa habis──> GameOver ──R──> Playing
```

- **Ready**: layar judul, skor terbaik, dan petunjuk kontrol.
- **Playing**: gameplay berjalan, hantu muncul setiap 0,7 detik.
- **Paused**: gameplay dan timer dibekukan; musik tetap berjalan.
- **GameOver**: ringkasan skor; tekan `R` untuk bermain lagi atau `ESC` untuk keluar.

---

## Mekanik Utama

- Gravitasi: `{0, -0.6}` per tick update 120 FPS.
- Lompat: kecepatan vertikal menjadi `9`.
- Hantu muncul di `x = 930` dengan `y` acak 100–500.
- Tipe hantu:
  - **Tipe 0** — lambat, semi-transparan.
  - **Tipe 1** — cepat (merah).
  - **Tipe 2** — kecepatan sedang, bergerak naik-turun (kuning).
- Deteksi tabrakan menggunakan jarak lingkaran:
  `jarak <= radius penyihir + radius hantu`.
- Setiap tabrakan atau keluar batas layar mengurangi 1 nyawa.
- Setelah terkena, pemain kebal selama 1,2 detik dan karakter berkedip.
- Skor bertambah saat hantu melewati sisi kiri layar.
- Highscore disimpan otomatis ke `highscore.txt` saat rekor baru tercapai,
  saat game over, dan saat game ditutup.

---

## Debug

Tekan `F3` untuk menampilkan informasi:
- state game saat ini,
- jumlah hantu aktif,
- FPS,
- posisi penyihir.

Debug overlay disembunyikan secara default agar tampilan tetap bersih.

---

## Troubleshooting

| Masalah | Solusi |
| --- | --- |
| `sliLoadFont() could not open font file` | *Working directory* salah. Jalankan dari folder `TheSorcerer` atau salin seluruh aset ke samping `.exe`. |
| Gagal membuka `sl.h` | Periksa `AdditionalIncludeDirectories`; default mengarah ke `D:\libs\sigil\include`. |
| *Link error* `sigil.lib` | Periksa `AdditionalLibraryDirectories` dan pastikan build menggunakan **x64**. |
| Aplikasi langsung tertutup / DLL tidak ditemukan | Salin `sigil.dll`, `OpenAL32.dll`, `wrap_oal.dll`, dan `glew32.dll` ke folder `.exe`. |
| Tidak ada suara | Pastikan `halloween.WAV` ada, `winmm.lib` ter-link, dan musik tidak di-toggle off dengan `M`. |
| Highscore tidak tersimpan | Pastikan folder *working directory* memiliki izin tulis. |

---

## Kredit

- Game dan kode: `bayusetiaji@amikom.ac.id`.
- Font: **Halloween Too** oleh Phelan Riessen, **F25 Bank Printer** oleh Volker Busse.
- Musik: **Halloween** dari CapCut audio library.
- Library: **SIGIL** oleh Geoff Nagy.
