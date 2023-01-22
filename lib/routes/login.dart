import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:go_router/go_router.dart';
import 'package:sivalingam_milks/jsontodart.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:http/http.dart';

class LoginForm extends StatefulWidget {
  const LoginForm({Key? key}) : super(key: key);

  @override
  LoginFormState createState() {
    return LoginFormState();
  }
}

class LoginFormState extends State<LoginForm> {
  TextEditingController mobileController = TextEditingController();
  TextEditingController passwordController = TextEditingController();

  final _mobileKey = GlobalKey<FormState>();
  final _passwordKey = GlobalKey<FormState>();

  get message => null;
  get messagecode => null;
  get token => null;
  get userinfo => null;
  addStringToSF(String mobile, String password, String token) async {
    SharedPreferences prefs = await SharedPreferences.getInstance();
    prefs.setString('mobile', mobile);
    prefs.setString('password', password);
    prefs.setString('token', token);
  }

  void loginapicall(String mobile, String password) async {
    try {
      Response response = await post(
          Uri.parse('https://sivalingammilks.com/api/login'),
          body: {'mobile': mobile, 'password': password});

      if (response.statusCode == 200) {
        // Loginuser(token: response.body));
        Loginuser.fromJson(jsonDecode(response.body));

//      print(data['token']);
//      print('Login successfully');

      } else {
//      print('failed');
      }
    } catch (e) {
//    print(e.toString());
    }
  }

  @override
  Widget build(BuildContext context) {
    // Build a Form widget using the _formKey created above.
    return Form(
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
//            autofocus: true,
            key: _passwordKey,
//            onChanged: (val) => _password.password = val,
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
                    addStringToSF(mobileController.text.toString(),
                        passwordController.text.toString(), token);
                    loginapicall(mobileController.text.toString(),
                        passwordController.text.toString());
                  }
                  mobileController.clear();
                  passwordController.clear();
                  context.go('/Home');
                },
                child: const Text('Submit')),
          ),
          Padding(
            padding: const EdgeInsets.symmetric(vertical: 16.0),
            child: ElevatedButton(
                onPressed: () => context.go('/signup'),
                child: const Text('Signup')),
          ),
        ],
      ),
    );
  }
}
