# Halloween

Kumpulan project C++ bertema Halloween yang dibangun dengan
[SIGIL](https://www.libsigil.com/) (Sound, Input, and Graphics Integration Library).

Project utama adalah **TheSorcerer**, sebuah game arkade bergaya *flappy bird*:
seorang penyihir harus terbang melewati hantu-hantu yang datang dari sisi kanan layar.

## Project Utama

### TheSorcerer

Game lengkap dengan layar judul, pause, sistem nyawa, skor, highscore tersimpan,
musik, efek suara, dan efek visual. Dokumentasi lengkap tersedia di
[`TheSorcerer/README.md`](TheSorcerer/README.md).

Kontrol singkat:

| Tombol | Aksi |
| --- | --- |
| `SPACE` | Mulai / terbang |
| `P` atau `ESC` | Pause / lanjutkan |
| `R` | Restart |
| `M` | Musik on/off |
| `F3` | Debug overlay |

## Project Pendukung

Project-project berikut adalah eksperimen kecil untuk menguji fungsionalitas yang
kemudian dipakai di `TheSorcerer`.

| Project | Tujuan |
| --- | --- |
| `Sorcerer` | Prototipe awal karakter penyihir, gravitasi, lompat, dan awan bergerak. |
| `RollingBG` | Uji latar `skynite.png` yang bergulir dan berulang tanpa celah. |
| `EventMod` | Uji `KeyListener` untuk membedakan tombol *pressed* dan *held*. |
| `PlayMusic` | Uji pemutaran musik latar secara loop dengan `PlaySound`. |
| `RemElm` | Uji `std::remove_if` + `erase` untuk menghapus elemen `vector`. |
| `TimeBasic` | Uji update berbasis akumulasi delta time. |

## Build

Prasyarat:

- Windows 10/11 x64.
- Visual Studio dengan **Desktop development with C++**.
- SIGIL di `D:\libs\sigil` (`include\sl.h`, `lib\sigil.lib`, dan DLL runtime).
- Toolset `v145` (ubah `PlatformToolset` di file `.vcxproj` jika memakai toolset lain).

Langkah:

1. Buka `Halloween.sln`.
2. Pilih **Release | x64**.
3. Build project yang diinginkan, lalu jalankan dari Visual Studio agar aset ditemukan.

Project utama hanya dikonfigurasi untuk **x64**. Untuk menjalankan `.exe` secara
langsung, salin aset dan DLL SIGIL ke folder yang sama dengan `.exe`.
Detail lengkap dan troubleshooting ada di [`TheSorcerer/README.md`](TheSorcerer/README.md).
