import 'package:flutter/material.dart';

void main() => runApp(const PilihPerangkatApp());

class PilihPerangkatApp extends StatelessWidget {
  const PilihPerangkatApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: PilihPerangkatPage(),
    );
  }
}

class PilihPerangkatPage extends StatelessWidget {
  final List<String> devices = [
    "Redmi 10C",
    "Advan Al Gen",
    "Samsung S23",
    "Realme C1",
    "ROG Phone",
    "Galic Instrument"
  ];

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Colors.white,
        elevation: 0, // Hilangkan bayangan bawah AppBar
        leading: IconButton(
          icon: const Icon(Icons.arrow_back),
          onPressed: () {
            // Action saat tombol back ditekan
          },
        ),
        title: const Text(
          "Pilih Perangkat",
          style: TextStyle(
            color: Colors.black,
          ),
        ),
        iconTheme: const IconThemeData(
          color: Colors.black, // Warna ikon
        ),
      ),
      backgroundColor: Colors.grey[200], // Warna background
      body: Padding(
        padding: const EdgeInsets.all(8.0),
        child: ListView.builder(
          itemCount: devices.length,
          itemBuilder: (context, index) {
            return Padding(
              padding: const EdgeInsets.symmetric(vertical: 8.0),
              child: Container(
                decoration: BoxDecoration(
                  color: Colors.white,
                  borderRadius: BorderRadius.circular(10),
                  boxShadow: [
                    BoxShadow(
                      color: Colors.black12,
                      blurRadius: 6,
                      offset: Offset(0, 2),
                    ),
                  ],
                ),
                child: ListTile(
                  leading: const Icon(Icons.phone_android),
                  title: Text(devices[index]),
                  onTap: () {
                    // Tindakan saat item dipilih
                  },
                ),
              ),
            );
          },
        ),
      ),
    );
  }
}
