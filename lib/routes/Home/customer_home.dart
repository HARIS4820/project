import 'package:flutter/material.dart';

//import 'package:http/http.dart' as http;
//import 'package:shared_preferences/shared_preferences.dart';
//import 'package:sivalingam_milks/jsontodart.dart';
//import 'dart:convert';
//import 'package:intl/intl.dart';
import 'package:sivalingam_milks/routes/pay_bill.dart';

class CustomerHome extends StatefulWidget {
  const CustomerHome({super.key});

  @override
  State<CustomerHome> createState() => _CustomerHomeState();
}

class _CustomerHomeState extends State<CustomerHome> {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
        title: 'Sivalingam Milks',
        home: Scaffold(
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
            body: Column(
              children: const [
                Text('Customer'),
                Unpaid(datalist: []),
              ],
            ),
            bottomNavigationBar: BottomAppBar(
              color: Colors.blue,
              child: Row(
                mainAxisSize: MainAxisSize.max,
                mainAxisAlignment: MainAxisAlignment.spaceAround,
                children: <Widget>[
                  IconButton(
                    icon: const Icon(Icons.home),
                    color: Colors.yellow,
                    onPressed: () {},
                  ),
                  IconButton(
                    icon: const Icon(Icons.date_range),
                    color: Colors.white,
                    onPressed: () {},
                  ),
                  IconButton(
                    icon: const Icon(Icons.payment),
                    color: Colors.white,
                    onPressed: () {},
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
            )));
  }
}
