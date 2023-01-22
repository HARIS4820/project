//  import 'package:flutter/foundation.dart';
import 'package:flutter_secure_storage/flutter_secure_storage.dart';

class SecureStorage {
  // Create storage
  final storage = const FlutterSecureStorage();

  final String _keyMobile = 'mobile';
  final String _keyPassWord = 'password';

  Future setUserName(String username) async {
    await storage.write(key: _keyMobile, value: username);
  }

  Future<String?> getUserName() async {
    return await storage.read(key: _keyMobile);
  }

  Future setPassWord(String password) async {
    await storage.write(key: _keyPassWord, value: password);
  }

  Future<String?> getPassWord() async {
    return await storage.read(key: _keyPassWord);
  }

  getMobile() {}
}
