import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:go_router/go_router.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:http/http.dart' as http;
import 'package:sivalingam_milks/routes/login_autofill.dart';

import '../jsontodart.dart';

class Verifytoken extends StatefulWidget {
  const Verifytoken({super.key});

  @override
  State<Verifytoken> createState() => _VerifytokenState();
}

class _VerifytokenState extends State<Verifytoken> {
  @override
  Widget build(BuildContext context) {
    return FutureBuilder(
      future: verifyToken(),
      builder: (BuildContext context, AsyncSnapshot<bool> snapshot) {
        if (snapshot.data ?? false) {
          // Token is valid, start the app
          context.go('/Home');
          return const Text('Login Sucessfully');
        } else {
          context.go('/Login');
          // Token is invalid or still being verified
          return const LoginForm();
        }
      },
    );
  }
}

Future<String?> getToken() async {
  final SharedPreferences prefs = await SharedPreferences.getInstance();
  prefs.getString('role');
  return prefs.getString('token');
}

Future<bool> verifyToken() async {
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
    return true;
  } else {
    return false;
  }
}
