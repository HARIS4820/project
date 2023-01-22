import 'package:flutter/material.dart';

class BmHome extends StatefulWidget {
  const BmHome({super.key});

  @override
  State<BmHome> createState() => _BmHomeState();
}

class _BmHomeState extends State<BmHome> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        centerTitle: true,
        title: const Text('Sivalingam Milks'),
      ),
      body: const Text('Branch Manager'),
    );
  }
}
