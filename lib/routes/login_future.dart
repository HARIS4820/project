import 'dart:convert';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:go_router/go_router.dart';
import 'package:sivalingam_milks/jsontodart.dart';
import 'package:http/http.dart' as http;
import 'package:shared_preferences/shared_preferences.dart';
// import 'package:sivalingam_milks/action/credtional_api.dart';

// Create a Form widget.
class LoginForm extends StatefulWidget {
  const LoginForm({Key? key}) : super(key: key);

  @override
  LoginFormState createState() {
    return LoginFormState();
  }
}

// Create a corresponding State class.
// This class holds data related to the form.
class LoginFormState extends State<LoginForm> {
  // Create a global key that uniquely identifies the Form widget
  // and allows validation of the form.
  //
  // Note: This is a GlobalKey<FormState>,
  // not a GlobalKey<LoginFormState>.
  TextEditingController mobileController = TextEditingController();
  TextEditingController passwordController = TextEditingController();
  // TextEditingController _qrTextEditingController = TextEditingController();
  final _mobileKey = GlobalKey<FormState>();
  final _passwordKey = GlobalKey<FormState>();
  //  final SecureStorage _secureStorage = SecureStorage();

  get message => null;

  get messagecode => null;

  get userinfo => null;

  get token => null;

  get prefs => null;

  Future<Loginuser?> loginuser(String mobile, String password) async {
    final response = await http.post(
        Uri.parse('https://sivalingammilks.com/api/login'),
        body: {'mobile': mobile, 'password': password});

    if (response.statusCode == 200) {
      Loginuser.fromJson(jsonDecode(response.body));
      Loginuser user = Loginuser.fromJson(json.decode(response.body));

      final prefs = await SharedPreferences.getInstance();
      await prefs.setString('token', user.token);

      // if (token != null) {
      // context.go('/Home');
      // }

    } else {
      throw Exception('Failed to update album.');
    }
    return null;
  }

  @override
  Widget build(BuildContext context) {
    // Build a Form widget using the _formKey created above.
    return MaterialApp(
      title: 'Sivalingam Milks',
      home: Scaffold(
        appBar: AppBar(
          centerTitle: true,
          title: const Text('Sivalingam Milks'),
        ),
        body: Form(
            key: _mobileKey,
            child: Column(
                crossAxisAlignment: CrossAxisAlignment.center,
                children: [
                  TextFormField(
                    controller: mobileController,
                    autofocus: true,
                    maxLength: 10,
                    inputFormatters: [
                      FilteringTextInputFormatter.digitsOnly,
                      LengthLimitingTextInputFormatter(10),
                    ],
                    textInputAction: TextInputAction.next,
                    keyboardType: TextInputType.phone,
                    // move to the next field
                    // onEditingComplete: _node.nextFocus,
                    decoration: const InputDecoration(
                      border: InputBorder.none,
                      hintText: "Enter Your Mobile Number",
                    ),
                    // The validator receives the text that the user has entered.
                    validator: (value) {
                      if (value == null || value.isEmpty) {
                        return 'Please enter your mobile number';
                      } else if (value.length < 10) {
                        return 'Enter Valid mobile number';
                      } else {
                        return null;
                      }
                    },
                  ),
                  TextFormField(
                    controller: passwordController,
                    autofocus: true,
                    key: _passwordKey,
                    maxLength: 30,
                    keyboardType: TextInputType.visiblePassword,
                    decoration: const InputDecoration(
                      border: InputBorder.none,
                      hintText: "Enter Your Password",
                    ),
                    // The validator receives the text that the user has entered.
                    validator: (value) {
                      if (value == null || value.isEmpty) {
                        return 'Please enter your password';
                      } else if (value.length < 8) {
                        return 'Minimum 8 digits rquired';
                      }
                      return null;
                    },
                  ),
                  Padding(
                    padding: const EdgeInsets.symmetric(vertical: 16.0),
                    child: ElevatedButton(
                        onPressed: () async {
                          // Validate returns true if the form is valid, or false otherwise.
                          if (_mobileKey.currentState!.validate()) {
                            await loginuser(mobileController.text.toString(),
                                    passwordController.text.toString())
                                .then((result) {
                              ScaffoldMessenger.of(context)
                                  .showSnackBar(const SnackBar(
                                content: Text('Failed'),
                              ));

                              context.go('/Home');
                            }).onError((error, stackTrace) {
                              //  print(error);
                            });
                          }
                        },
                        child: const Text('Submit')),
                  ),
                ])),
      ),
    );
  }
}
