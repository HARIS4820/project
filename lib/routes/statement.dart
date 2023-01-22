import 'package:flutter/material.dart';
import 'package:go_router/go_router.dart';

class StatementPage extends StatelessWidget {
  const StatementPage({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        appBar: AppBar(
          centerTitle: true,
          title: const Text('Sivalingam Milks'),
          leading: Builder(
              builder: ((context) => IconButton(
                    icon: const Icon(Icons.menu),
                    color: Colors.white,
                    onPressed: () => Scaffold.of(context).openDrawer(),
                  ))),
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
        drawer: const Drawer(
          backgroundColor: Colors.blue,
        ),
        body: const Text("Home"),
        bottomNavigationBar: BottomAppBar(
          color: Colors.blue,
          child: Row(
            mainAxisSize: MainAxisSize.max,
            mainAxisAlignment: MainAxisAlignment.spaceAround,
            children: <Widget>[
              IconButton(
                icon: const Icon(Icons.home),
                color: Colors.green,
                onPressed: () {
                  context.go('/Home');
                },
              ),
              IconButton(
                icon: const Icon(Icons.date_range),
                color: Colors.white,
                onPressed: () {
                  context.go('/Statement');
                },
              ),
              IconButton(
                icon: const Icon(Icons.payment),
                color: Colors.white,
                onPressed: () {
                  context.go('/payment');
                },
              ),
              IconButton(
                icon: const Icon(Icons.support_agent),
                color: Colors.white,
                onPressed: () {},
              ),
              IconButton(
                icon: const Icon(Icons.account_circle),
                color: Colors.white,
                onPressed: () {},
              ),
            ],
          ),
        ));
  }
}
