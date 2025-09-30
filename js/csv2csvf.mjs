export function csvToCsvf(data) {
 let csvf = "";
 let xd = data[0].split(",").length;
 while(xd--) {
  csvf += "1,";
 }
 csvf += "1\n";
 for(let i = 1; i < data.length; i++) {
  csvf += "1\n"+data[i]+"\n";
 }
 csvf += "1";
 return csvf;
}