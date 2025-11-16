# ⚙️ Tips & Trik Kodingan IoT

File ini berisi kumpulan tips dan praktik baik dalam menulis kode untuk proyek IoT, agar sistem berjalan stabil, efisien, dan mudah dikembangkan.

---

## 🕒 1. Hindari `delay()`, Gunakan `millis()`

Fungsi `delay()` menghentikan seluruh program untuk waktu tertentu, membuat sistem tidak bisa membaca sensor lain atau berkomunikasi selama jeda itu.  
Gunakan `millis()` agar proses lain tetap berjalan paralel.

### Contoh Penggunaan `millis()` dengan Fungsi Terpisah

```cpp
unsigned long prevMillis = 0;
const long interval = 1000; // jeda 1 detik

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (checkInterval(interval)) {
    Serial.println("1 detik berlalu tanpa delay()");
  }
}

bool checkInterval(long duration) {
  unsigned long currentMillis = millis();
  if (currentMillis - prevMillis >= duration) {
    prevMillis = currentMillis;
    return true;
  }
  return false;
}
```
<br>

##  🔄 2. Gunakan State Machine / Flag

Gunakan flag atau state machine untuk mengatur logika kompleks tanpa delay().
Pendekatan ini menjaga program tetap responsif meski banyak kondisi yang harus dicek.

### Contoh:
```cpp
bool ledState = false;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (checkInterval(500)) { // pakai fungsi dari tips sebelumnya
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);
  }
}
```
<br>

## 🧩 3. Pisahkan Fungsi untuk Setiap Tugas

Struktur program yang rapi memudahkan debugging dan pengembangan.  
Gunakan fungsi terpisah untuk membaca sensor, mengirim data, atau menampilkan hasil.

### Contoh

```cpp
void setup() {
  Serial.begin(9600);
}

void loop() {
  readSensor();
  sendData();
  displayInfo();
  handleTiming();
}

void readSensor() {
  // Contoh baca sensor
  int value = analogRead(A0);
  Serial.print("Sensor: ");
  Serial.println(value);
}

void sendData() {
  // Simulasi kirim data ke server
  Serial.println("Mengirim data ke server...");
}

void displayInfo() {
  // Simulasi tampilan LCD atau Serial
  Serial.println("Menampilkan data ke user...");
}

void handleTiming() {
  // Tempat untuk fungsi berbasis millis()
}
```

## 🧠 4. Gunakan Serial Monitor untuk Debugging

Selalu manfaatkan Serial.print() untuk memantau nilai variabel, kondisi sensor, atau alur program.
Namun, hapus atau komentari bagian ini di versi final agar tidak memperlambat sistem.

### Contoh:
```cpp
int sensorValue = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  sensorValue = analogRead(A0);
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);
  delay(500);
}
```

## 🌐 5. Gunakan Komentar dengan Jelas

Komentar bukan hiasan.
Gunakan untuk menjelaskan maksud logika yang rumit, tapi hindari komentar yang hanya mengulang isi kode.

### Contoh yang Baik:
```cpp
// Cek apakah suhu melewati batas maksimal
if (temperature > 35) {
  sendAlert();
}
```


## ⚙️ 6. Contoh Penggunaan Flag / State Machine

Daripada menggunakan `delay()` untuk menunggu kondisi tertentu, lebih baik gunakan **flag** atau **state machine** agar program tetap responsif dan tidak terhenti.

`delay()` akan menghentikan seluruh eksekusi program selama waktu yang ditentukan.  
Kalau kamu ingin LED berkedip setiap 500 ms **tanpa menghentikan tugas lain**, gunakan logika berbasis waktu (`millis()`) dan status (`flag`).

---

### Contoh:

```cpp
bool ledOn = false;               // Menyimpan status LED
unsigned long prevMillisLed = 0;  // Waktu terakhir LED berubah

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);   // Set pin LED sebagai output
}

void loop() {
  unsigned long now = millis();   // Ambil waktu saat ini (dalam ms)

  // Jika sudah lewat 500 ms, ubah status LED
  if (now - prevMillisLed >= 500) {
    prevMillisLed = now;          // Perbarui waktu terakhir perubahan
    ledOn = !ledOn;               // Balikkan status LED
    digitalWrite(LED_BUILTIN, ledOn);
  }

  // Tugas lain tetap bisa dijalankan di bawah ini
}
```
