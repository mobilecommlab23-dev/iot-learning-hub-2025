# Troubleshooting Project IoT

## 1. Masalah Koneksi Board

| Gejala | Penyebab Umum | Solusi |
|--------|----------------|--------|
| Port USB tidak muncul di Arduino IDE | Driver belum terinstal / kabel hanya power | Instal driver **CH340** atau **CP210x**, ganti kabel USB data |
| Upload program gagal | Salah pilih board atau port | Pastikan board dan port sesuai di menu **Tools** |
| Tidak bisa upload ke ESP32 | Boot mode tidak aktif | Tekan dan tahan tombol **BOOT** saat upload |

---

## 2. Masalah Koneksi Wi-Fi

| Gejala | Penyebab Umum | Solusi |
|--------|----------------|--------|
| Board gagal terhubung ke Wi-Fi | SSID atau password salah | Periksa kembali kredensial Wi-Fi di kode |
| Wi-Fi konek tapi data tidak terkirim | IP konflik atau sinyal lemah | Gunakan jaringan stabil, cek `WiFi.status()` |
| Sering disconnect otomatis | Router DHCP delay / sinyal drop | Gunakan IP statis atau perpanjang interval reconnect |

---

## 3. Masalah Komunikasi MQTT

| Gejala | Penyebab Umum | Solusi |
|--------|----------------|--------|
| Gagal connect ke broker | Hostname / port salah | Pastikan broker MQTT aktif dan alamat benar |
| Tidak ada pesan yang diterima | Topic tidak cocok / salah subscribe | Cek kesesuaian topic publish dan subscribe |
| Data tidak tampil di dashboard | Format payload tidak sesuai | Pastikan data dikirim dalam format string atau JSON |

---

## 4. Masalah pada Sensor

| Gejala | Penyebab Umum | Solusi |
|--------|----------------|--------|
| Nilai sensor tidak muncul | Pin salah / library belum diinstal | Periksa wiring dan library yang digunakan |
| Nilai tidak berubah | Sensor rusak / tidak diberi delay | Tambahkan jeda pembacaan (`delay()`) yang sesuai |
| Data aneh / tidak stabil | Noise atau pin floating | Gunakan resistor pull-up atau stabilisasi daya |

---

## 5. Masalah pada Aktuator

| Gejala | Penyebab Umum | Solusi |
|--------|----------------|--------|
| LED / relay / servo tidak merespons | Pin output salah / arus tidak cukup | Pastikan pin dan catu daya benar |
| Servo bergetar terus | Sinyal PWM tidak stabil | Gunakan library Servo dan pin PWM yang sesuai |
| Relay berisik / aktif terus | Salah logika HIGH-LOW | Cek tipe relay (aktif HIGH atau LOW) dan ubah kode logikanya |

---

## 6. Masalah Daya & Perangkat

| Gejala | Penyebab Umum | Solusi |
|--------|----------------|--------|
| Board sering restart | Catu daya kurang kuat | Gunakan adaptor 5V 2A atau sumber daya stabil |
| Sensor tidak terbaca | Ground tidak tersambung | Pastikan semua komponen memiliki GND yang sama |
| Board panas | Arus berlebih / wiring salah | Periksa sambungan dan gunakan regulator tegangan |

---

## 7. Masalah Software & Library

| Gejala | Penyebab Umum | Solusi |
|--------|----------------|--------|
| Error saat compile | Versi library tidak cocok | Hapus dan reinstall library terbaru |
| Include file tidak ditemukan | Path library salah | Pastikan library ada di folder `Documents/Arduino/libraries` |
| IDE crash / lambat | Versi IDE bermasalah | Update ke versi terbaru Arduino IDE |

---

## 8. Tips Umum Debugging

- Gunakan **Serial Monitor** untuk mencetak log dan status koneksi.  
- Tambahkan `Serial.println()` di bagian penting kode untuk melacak proses.  
- Uji tiap komponen (sensor, aktuator) **secara terpisah** sebelum digabung.  

---