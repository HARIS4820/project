import 'package:flutter/material.dart';
// import 'package:sivalingam_milks/action/verify_token.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:sivalingam_milks/jsontodart.dart';
//import 'package:sivalingam_milks/routes/Home/home_page.dart';
import 'package:sivalingam_milks/routes/login_autofill.dart';
import 'package:http/http.dart' as http;
import 'package:sivalingam_milks/routes/Home/customer_home.dart';
import 'package:sivalingam_milks/routes/Home/sale_home.dart';
import 'package:sivalingam_milks/routes/Home/bm_home.dart';
import 'package:sivalingam_milks/routes/Home/admin_home.dart';

import 'dart:convert';

void main() => runApp(const MyApp());

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      initialRoute: '/',
      routes: {
        '/': (context) => const Screen(),
        // '/Login': (context) => const LoginForm(),
        // '/Home': (context) => const HomeScreen(),
      },
    );
  }
}

Future<String?> getToken() async {
  final SharedPreferences prefs = await SharedPreferences.getInstance();
  prefs.getString('role');
  return prefs.getString('token');
}

Future<Loginuser?> verifyToken() async {
  String? token = await getToken();

  final response = await http
      .get(Uri.parse('https://sivalingammilks.com/api/verify'), headers: {
    'Content-Type': 'application/json',
    'Accept': 'application/json',
    'Authorization': 'Bearer $token',
  });
  if (response.statusCode == 200) {
    Loginuser.fromJson(jsonDecode(response.body));
    Loginuser user = Loginuser.fromJson(json.decode(response.body));

    final prefs = await SharedPreferences.getInstance();
    await prefs.setString('token', user.token);
    return user;
  } else {
    return null;
  }
}

class Screen extends StatelessWidget {
  const Screen({super.key});

  @override
  Widget build(BuildContext context) {
    return FutureBuilder(
        future: verifyToken(),
        builder: (BuildContext context, AsyncSnapshot<Loginuser?> snapshot) {
          if (snapshot.data != null) {
            if (snapshot.data?.userInfo.role == 'admin') {
              // Do something with the data
              return const AdminHome();
            } else if (snapshot.data?.userInfo.role == 'branchmanager') {
              // Do something with the data
              return const BmHome();
            } else if (snapshot.data?.userInfo.role == 'salesman') {
              // Do something with the data
              return const SaleHome();
            } else if (snapshot.data?.userInfo.role == 'customer') {
              // Do something with the data
              return const CustomerHome();
            }
            return const CircularProgressIndicator();
          } else {
            // Token is invalid or still being verified
            return const LoginForm();
          }
        });
  }
}
