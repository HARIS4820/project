import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'package:shared_preferences/shared_preferences.dart';
import 'package:sivalingam_milks/jsontodart.dart';
import 'dart:convert';
import 'package:intl/intl.dart';

Future<String?> getToken() async {
  final SharedPreferences prefs = await SharedPreferences.getInstance();
  return prefs.getString('token');
}

Future<List<UnpaidDetails>> unpaid() async {
  String? token = await getToken();

  final response = await http.get(
      Uri.parse('https://sivalingammilks.com/api/payment/unpaid'),
      headers: {
        'Content-Type': 'application/json',
        'Accept': 'application/json',
        'Authorization': 'Bearer $token',
      });

  if (response.statusCode == 200) {
    var result = jsonDecode(response.body);
    List jsonResponse = result["details"] as List;

    return jsonResponse.map((e) => UnpaidDetails.fromJson(e)).toList();
  } else {
    throw Exception('Failed to update album.');
  }
}

class Unpaid extends StatefulWidget {
  const Unpaid({Key? key, required this.datalist}) : super(key: key);
  final List<UnpaidDetails> datalist;
  @override
  UnpaidState createState() => UnpaidState();
}

class UnpaidState extends State<Unpaid> {
  @override
  Widget build(BuildContext context) {
    return SingleChildScrollView(
      child: Center(
        child: Column(children: [
          const Text("Pay Bills"),
          Padding(
            padding: const EdgeInsets.all(8.0),
            child: FutureBuilder(
              future: unpaid(),
              builder: (BuildContext context,
                  AsyncSnapshot<List<UnpaidDetails>> snapshot) {
                if (snapshot.hasData) {
                  List<UnpaidDetails> sortedData = snapshot.data!
                    ..sort((b, a) => a.year.compareTo(b.year) == 0
                        ? a.month.compareTo(b.month)
                        : a.year.compareTo(b.year));
                  return DataTable(
                    decoration: BoxDecoration(
                      color: Colors.grey.shade100,
                      border: Border.all(color: Colors.grey),
                      borderRadius: BorderRadius.circular(10),
                    ),
                    dataRowColor: MaterialStateProperty.all(Colors.white),
                    columns: const [
                      DataColumn(
                        label: Text('   Perid',
                            textAlign: TextAlign.right,
                            style: TextStyle(fontWeight: FontWeight.bold)),
                      ),
                      DataColumn(
                          label: Text('Type',
                              textAlign: TextAlign.center,
                              style: TextStyle(fontWeight: FontWeight.bold))),
                      DataColumn(
                          label: Text('     Pay',
                              textAlign: TextAlign.right,
                              style: TextStyle(fontWeight: FontWeight.bold))),
                    ],
                    rows: sortedData
                        .map((UnpaidDetails unpaid) => DataRow(cells: [
                              DataCell(FittedBox(
                                child: Text(
                                    "${DateFormat.MMM().format(DateTime(2022, unpaid.month))} - ${unpaid.year}",
                                    textAlign: TextAlign.center,
                                    style: const TextStyle(
                                        fontWeight: FontWeight.bold)),
                              )),
                              // DataCell(Text('${unpaid.month}-${unpaid.year}')),
                              DataCell(FittedBox(
                                child: Text(unpaid.type,
                                    textAlign: TextAlign.center,
                                    style: const TextStyle(
                                        fontWeight: FontWeight.bold)),
                              )),
                              DataCell(SizedBox(
                                width: 80.0,
                                child: ElevatedButton(
                                    onPressed: () {},
                                    child: FittedBox(
                                      child: Text(
                                        'Rs.${unpaid.price}',
                                        textAlign: TextAlign.center,
                                      ),
                                    )),
                              )),
                            ]))
                        .toList(),
                  );
                } else if (snapshot.hasError) {
                  return Text("${snapshot.error}");
                }
                return const CircularProgressIndicator();
              },
            ),
          )
        ]),
      ),
    );
  }
}
