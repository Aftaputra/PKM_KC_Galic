import 'package:flutter/material.dart';

void main() => runApp(const MyApp());

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(title: const Text('Row Example')),
        body: const Center(
          child: CustomRowWidget(),
        ),
      ),
    );
  }
}

class CustomRowWidget extends StatelessWidget {
  const CustomRowWidget({super.key});

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 16.0),
      child: Row(
        mainAxisAlignment: MainAxisAlignment.spaceBetween,
        children: [
          // Text 1
          Text(
            'Terhubung ke : ',
            style: TextStyle(
              fontSize: 16,
              fontWeight: FontWeight.normal,
            ),
          ),
          // Text 2
          Expanded(
            child: Text(
              'Galic Instrument',
              style: TextStyle(
                fontSize: 16,
                fontWeight: FontWeight.bold,
                decoration: TextDecoration.underline,
              ),
              textAlign: TextAlign.center,
            ),
          ),
          // Button outline
          OutlinedButton(
            onPressed: () {
              // Aksi ketika tombol ditekan
              print('Ganti');
            },
            child: const Text('Ganti'),
          ),
        ],
      ),
    );
  }
}
