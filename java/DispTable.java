import java.util.Arrays;
import java.util.List;
import java.io.*;
import java.nio.file.*;

public class DispTable {
 private String[] file; // CSVF file split by newlines
 private int[] cellSize; // For each column, the max characters in the cells
 private byte[] colThick; // List of each of the columns' thicknesses [1 or 2]
 public String table; // The final result, no need to add getters and setters
 
 /* THE MAIN METHOD
  Reads in a file and converts it to a `DispTable` object, and prints out the table outputted.
 */
 public static void main(String[] args) {
 // If there is the correct number of arguments,
  if(args.length != 1) {
   System.out.println("\1n\033[1;31mError: no file selected\033[0m\n\n\033[4mCommand usage: java DispTable file.csvf\033[0m\n");
   System.exit(-1);
  }
  // Try reading the file.
  try {
   List<String> linesList = Files.readAllLines(Paths.get(args[0]));
   String[] arr = linesList.toArray(new String[0]);
   // If the read is a success, try converting it.
   System.out.println(new DispTable(arr).table);
  } catch(IOException e) {
   System.out.println("\n\033[1;31mError: file "+e.getMessage()+" not found\033[0m\n\n\033[4mCommand usage: java DispTable file.csvf\033[0m\n");
   System.exit(-1);
  }
 }

/* THE CONSTRUCTOR
  Reads in the list of lines in a csvf file, and outputs a formatted table to `table`.
*/
 public DispTable(String[] data) {
 // initialize variables.
  file = data;
  cellSize = new int[file[2].split(",").length];
  colThick = new byte[file[0].split(",").length];
 
 // `cellSize` should be the maximum number of characters in each column.
  for(int i = 2; i < file.length; i += 2) {
   String[] cols = file[i].split(",");
   for(int j = 0; j < cols.length; j++) {
    if(cols[j].length() > cellSize[j]) {
     cellSize[j] = cols[j].length();
    }
   }
  }

// `colThick` is simply each value in the first row.
  for(int i = 0; i < colThick.length; i++) {
   colThick[i] = Byte.parseByte(file[0].split(",")[i]);
  }

//  DEBUG
//  System.out.println(Arrays.toString(cellSize)+"\n"+Arrays.toString(colThick));

  // Choose a different character based on row thickness and column thickness.
  switch(file[1]+colThick[0]) {
   case "11": table = "┌"; break; // row = 1, col = 1
   case "12": table = "╓"; break; // row = 1, col = 2
   case "21": table = "╒"; break; // row = 2, col = 1
   case "22": table = "╔"; break; // row = 2, col = 2
  }
  table += horizontal(cellSize[0], Byte.parseByte(file[1]));
  if(colThick.length != 2) {
  // Repeat for each column:
   for(int i = 1; i < (colThick.length-1); i++) {
    // Choose a character based on row/col thickness
    switch(file[1]+colThick[i]) {
     case "11": table += "┬"; break;
     case "12": table += "╥"; break;
     case "21": table += "╤"; break;
     case "22": table += "╦"; break;
    }
    // Add a line across to the next cell.
    table += horizontal(cellSize[i], Byte.parseByte(file[1]));
   }
  }
  // Final character of top row.
  switch(file[1]+colThick[colThick.length-1]) {
   case "11": table += "┐"; break;
   case "12": table += "╖"; break;
   case "21": table += "╕"; break;
   case "22": table += "╗"; break;
  }
  // On the next line, print each string in the top row of the table
  table += "\n"+vertical(file[2].split(","));
  
  // Repeat for each row
  for(int i = 3; i < (file.length-2); i += 2) {
   // Do the same as top row but use characters for the middle of the table
   table += "\n";
   switch(file[i]+colThick[0]) {
    case "11": table += "├"; break;
    case "12": table += "╟"; break;
    case "21": table += "╞"; break;
    case "22": table += "╠"; break;
   }
   table += horizontal(cellSize[0], Byte.parseByte(file[i]));
   for(int j = 1; j < (colThick.length-1); j++) {
    switch(file[i]+colThick[j]) {
     case "11": table += "┼"; break;
     case "12": table += "╫"; break;
     case "21": table += "╪"; break;
     case "22": table += "╬"; break;
    }
    table += horizontal(cellSize[j], Byte.parseByte(file[i]));
   }
   switch(file[i]+colThick[colThick.length-1]) {
    case "11": table += "┤"; break;
    case "12": table += "╢"; break;
    case "21": table += "╡"; break;
    case "22": table += "╣"; break;
   }
   table += "\n"+vertical(file[i+1].split(","));
  }
  // Do the same as the top row but with characters for the bottom row.
  table += "\n";
  switch(file[file.length-1]+colThick[0]) {
   case "11": table += "└"; break;
   case "12": table += "╙"; break;
   case "21": table += "╘"; break;
   case "22": table += "╚"; break;
  }
  table += horizontal(cellSize[0], Byte.parseByte(file[file.length-1]));
  for(int i = 1; i < (colThick.length-1); i++) {
   switch(file[file.length-1]+colThick[i]) {
    case "11": table += "┴"; break;
    case "12": table += "╨"; break;
    case "21": table += "╧"; break;
    case "22": table += "╩"; break;
   }
   table += horizontal(cellSize[i], Byte.parseByte(file[file.length-1]));
  }
  switch(file[file.length-1]+colThick[colThick.length-1]) {
   case "11": table += "┘"; break;
   case "12": table += "╜"; break;
   case "21": table += "╛"; break;
   case "22": table += "╝"; break;  
  }
  /* At the end, table should have a formatted table stored in it. EX:
    ┌─╥──┬──┬──┐
    │a║b │cc│d │
    ╞═╬══╪══╪══╡
    │b║d │dd│cc│
    ├─╫──┼──┼──┤
    │c║cc│c │c │
    ├─╫──┼──┼──┤
    │8║0 │b │v │
    ├─╫──┼──┼──┤
    │a║a │a │a │
    └─╨──┴──┴──┘
  */
 }

 // The line between each cell. 
 private String horizontal(int len, byte thick) {
  String out = "";
  for(int i = 0; i < len; i++) {
   out += (thick == 1) ? "─" : "═";
  }
  return out;
 }

 // Print each value in the row
 private String vertical(String[] values) {
  String out = "";
  for(int i = 0; i < colThick.length; i++) {
   out += (colThick[i] == 1) ? "│" : "║";
   if(i != colThick.length-1) {
    // Calculate how many spaces to pad the cell with
    int difference = cellSize[i] - values[i].length();
    out += values[i];
    if(difference != 0) { for(int j = 0; j < difference; j++) { out += " "; } }
   }
  }
  return out;
 }
}
