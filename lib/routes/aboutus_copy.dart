import 'package:flutter/material.dart';

class Hometest extends StatelessWidget {
  const Hometest({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Sivalingam Milks',
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Sivalingam Milks'),
          leading: IconButton(
            icon: const Icon(Icons.account_circle_rounded),
            color: Colors.white,
            onPressed: () {},
          ),
          actions: [
            IconButton(
              icon: const Icon(Icons.notifications),
              color: Colors.white,
              onPressed: () {},
            ),
          ],
          backgroundColor: Colors.blue,
          foregroundColor: Colors.white,
        ),
        body: Column(
          children: const [W1(), W2(), W3()],
        ),
        bottomNavigationBar: BottomNavigationBar(
          items: const <BottomNavigationBarItem>[
            BottomNavigationBarItem(
              icon: Icon(Icons.home),
              label: 'Home',
              backgroundColor: Colors.blue,
            ),
            BottomNavigationBarItem(
                icon: Icon(Icons.date_range),
                label: 'Statement',
                backgroundColor: Colors.blue),
            BottomNavigationBarItem(
                icon: Icon(Icons.payment),
                label: 'Pay Bill',
                backgroundColor: Colors.blue),
            BottomNavigationBarItem(
                icon: Icon(Icons.support_agent),
                label: 'Contact Us',
                backgroundColor: Colors.blue),
            BottomNavigationBarItem(
              icon: Icon(Icons.account_circle),
              label: 'Profile',
            ),
          ],
        ),
      ),
    );
  }
}

class W1 extends StatefulWidget {
  const W1({super.key});

  @override
  State<W1> createState() => _W1State();
}

class _W1State extends State<W1> {
  @override
  Widget build(BuildContext context) {
    return Container();
  }
}

class W2 extends StatefulWidget {
  const W2({super.key});

  @override
  State<W2> createState() => _W2State();
}

class _W2State extends State<W2> {
  @override
  Widget build(BuildContext context) {
    return Container();
  }
}

class W3 extends StatefulWidget {
  const W3({super.key});

  @override
  State<W3> createState() => _W3State();
}

class _W3State extends State<W3> {
  @override
  Widget build(BuildContext context) {
    return Container();
  }
}
