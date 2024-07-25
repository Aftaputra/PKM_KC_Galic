import 'package:flutter/material.dart';

void main() => runApp(const WarningCardApp());

class WarningCardApp extends StatelessWidget {
  const WarningCardApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        backgroundColor: Colors.grey[200],
        body: const Center(
          child: WarningCard(),
        ),
      ),
    );
  }
}

class WarningCard extends StatelessWidget {
  const WarningCard({super.key});

  @override
  Widget build(BuildContext context) {
    return Container(
      width: 350, // Atur lebar container
//       padding: const EdgeInsets.all(16.0),
      decoration: BoxDecoration(
        color: Colors.white,
        borderRadius: BorderRadius.circular(16),
      ),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.center,
        mainAxisSize: MainAxisSize.min,
        children: <Widget>[
          // Bagian atas - Peringatan (penuh tanpa padding)
          Container(
            padding: const EdgeInsets.symmetric(vertical: 8.0),
            width: double.infinity,
            decoration: BoxDecoration(
              color: Colors.orange[100],
              borderRadius: BorderRadius.vertical(top: Radius.circular(16)),
            ),
            child: const Center(
              child: Text(
                'Peringatan!',
                style: TextStyle(
                  fontSize: 16,
                  fontWeight: FontWeight.bold,
                  color: Colors.red,
                ),
              ),
            ),
          ),
//           const SizedBox(height: 16),
          // Container untuk Kadar Fe3+ dan informasi detail
          Container(
            padding: const EdgeInsets.all(16.0),
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.center,
              children: [
                // Kadar Fe3+
                const Text(
                  'Kadar Fe3+',
                  style: TextStyle(
                    fontSize: 18,
                    fontWeight: FontWeight.bold,
                    color: Colors.black,
                  ),
                ),
                const SizedBox(height: 16),
                // Row untuk 21.3 dan Detail Information with Icons
                Row(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: <Widget>[
                    // Nilai 21.3
                    const Text(
                      '21.3',
                      style: TextStyle(
                        fontSize: 56,
                        fontWeight: FontWeight.w900,
                        color: Colors.black,
                      ),
                    ),
                    const SizedBox(width: 24),
                    // Kolom untuk teks dan ikon
                    Expanded(
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Row(
                            children: <Widget>[
                              const Icon(Icons.location_on, color: Colors.black, size: 12.0),
                              const SizedBox(width: 8),
                              Flexible(
                                child: const Text(
                                  'Jl. STM Pembangunan No 21',
                                  style: TextStyle(
                                      fontSize: 12, color: Colors.black),
                                ),
                              ),
                            ],
                          ),
                          const SizedBox(height: 8),
                          Row(
                            children: <Widget>[
                              const Icon(Icons.calendar_today,
                                  color: Colors.black, size: 12.0),
                              const SizedBox(width: 8),
                              Flexible(
                                child: const Text(
                                  'Jum, 26 Juli 2024, 09.21',
                                  style: TextStyle(
                                      fontSize: 12, color: Colors.black),
                                ),
                              ),
                            ],
                          ),
                          const SizedBox(height: 8),
                          Row(
                            children: <Widget>[
                              const Icon(Icons.account_circle,
                                  color: Colors.black, size: 12.0),
                              const SizedBox(width: 8),
                              Flexible(
                                child: const Text(
                                  'Laksani',
                                  style: TextStyle(
                                      fontSize: 12, color: Colors.black),
                                ),
                              ),
                            ],
                          ),
                        ],
                      ),
                    ),
                  ],
                ),
              ],
            ),
          ),
        ],
      ),
    );
  }
}
