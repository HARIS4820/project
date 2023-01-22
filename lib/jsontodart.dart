class Loginuser {
  Loginuser({
    required this.message,
    required this.messageCode,
    required this.token,
    required this.userInfo,
  });
  late final String message;
  late final String messageCode;
  late final String token;
  late final UserInfo userInfo;

  Loginuser.fromJson(Map<String, dynamic> json) {
    message = json['message'];
    messageCode = json['messageCode'];
    token = json['token'];
    userInfo = UserInfo.fromJson(json['userInfo']);
  }

  Map<String, dynamic> toJson() {
    final data = <String, dynamic>{};
    data['message'] = message;
    data['messageCode'] = messageCode;
    data['token'] = token;
    data['userInfo'] = userInfo.toJson();
    return data;
  }
}

class UserInfo {
  UserInfo({
    required this.name,
    required this.role,
    required this.branchcode,
  });
  late final String name;
  late final String role;
  late final int? branchcode;

  UserInfo.fromJson(Map<String, dynamic> json) {
    name = json['name'];
    role = json['role'];
    branchcode = json['branchcode'] as int?;
  }

  Map<String, dynamic> toJson() {
    final data = <String, dynamic>{};
    data['name'] = name;
    data['role'] = role;
    data['branchcode'] = branchcode;
    return data;
  }
}

class Verify {
  Verify({
    required this.message,
    required this.messageCode,
    required this.token,
    required this.userInfo,
  });
  late final String message;
  late final String messageCode;
  late final String token;
  late final UserInfo userInfo;

  Verify.fromJson(Map<String, dynamic> json) {
    message = json['message'];
    messageCode = json['messageCode'];
    token = json['token'];
    userInfo = UserInfo.fromJson(json['userInfo']);
  }

  Map<String, dynamic> toJson() {
    final data = <String, dynamic>{};
    data['message'] = message;
    data['messageCode'] = messageCode;
    data['token'] = token;
    data['userInfo'] = userInfo.toJson();
    return data;
  }
}

class UnpaidDetails {
  final int price;
  final int month;
  final int year;
  final String type;

  UnpaidDetails(
      {required this.price,
      required this.month,
      required this.year,
      required this.type});

  factory UnpaidDetails.fromJson(Map<String, dynamic> json) {
    return UnpaidDetails(
        price: json['price']! as int,
        month: json['month']! as int,
        year: json['year']! as int,
        type: json['type']! as String);
  }
}
