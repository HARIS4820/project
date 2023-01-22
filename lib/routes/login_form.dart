import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:go_router/go_router.dart';
//import 'package:sivalingam_milks/user_input.dart';

// import 'package:http/http.dart';
import 'package:shared_preferences/shared_preferences.dart';

//import 'package:sivalingam_milks/routes/aboutus.dart';

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

  final _mobileKey = GlobalKey<FormState>();
  final _passwordKey = GlobalKey<FormState>();

  addStringToSF(String mobile, String password) async {
    SharedPreferences prefs = await SharedPreferences.getInstance();
    prefs.setString('mobile', mobile);
    prefs.setString('password', password);
  }

  @override
  void dispose() {
    mobileController.dispose();
    passwordController.dispose();
    super.dispose();
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
                onPressed: () {
                  // Validate returns true if the form is valid, or false otherwise.
                  if (_mobileKey.currentState!.validate()
                      //  &&
                      //     _passwordKey.currentState!.validate()
                      ) {
                    addStringToSF(mobileController.text.toString(),
                        passwordController.text.toString());
                    // loginapicall(mobileController.text.toString(),
                    //     passwordController.text.toString());
                  }
                  // mobileController.clear();
                  // passwordController.clear();
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
