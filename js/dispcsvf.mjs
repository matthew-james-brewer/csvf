let cellSize; // number[]
let colThick; // number[]

export function csvf_char_gen(data) {
 let table = 0;
 
 const d2split = data[2].split(",");
 const d0split = data[0].split(",");
 cellSize = new Array(d2split.length).fill(0);
 colThick = new Array(d0split.length).fill(0);
 for(let i = 2; i < data.length; i++) {
  let cols = data[i].split(",");
  for(let j = 0; j < cols.length; j++) {
   if(cols[j].length > cellSize[j]) {
    cellSize[j] = cols[j].length;
   }
  }
 }
 for(let i = 0; i < colThick.length; i++) {
  colThick[i] = d0split[i];
 }

 switch(data[1]+colThick[0]) {
  case "11": table = "┌"; break; // row = 1, col = 1
  case "12": table = "╓"; break; // row = 1, col = 2
  case "21": table = "╒"; break; // row = 2, col = 1
  case "22": table = "╔"; break; // row = 2, col = 2
 }

 table += horizontal(cellSize[0],+data[1]);

 if(colThick.length != 2) {
  for(let i = 1; i < colThick.length-1; i++) {
   switch(data[1]+colThick[i]) {
    case "11": table += "┬"; break;
    case "12": table += "╥"; break;
    case "21": table += "╤"; break;
    case "22": table += "╦"; break;
   }
   table += horizontal(cellSize[i], +data[1]);
  }
 }

 switch(data[1]+colThick[colThick.length-1]) {
  case "11": table += "┐"; break;
  case "12": table += "╖"; break;
  case "21": table += "╕"; break;
  case "22": table += "╗"; break;
 }
 table+= "\n"+vertical(d2split);

 for(let i = 3; i < data.length-2; i += 2) {
  table += "\n";
  switch(data[i]+colThick[0]) {
   case "11": table += "├"; break;
   case "12": table += "╟"; break;
   case "21": table += "╞"; break;
   case "22": table += "╠"; break;
  }
  table += horizontal(cellSize[0], data[i]);
  for(let j = 1; j < colThick.length-1; j++) {
   switch(data[i]+colThick[j]) {
    case "11": table += "┼"; break;
    case "12": table += "╫"; break;
    case "21": table += "╪"; break;
    case "22": table += "╬"; break;
   }
   table += horizontal(cellSize[j], data[i]);
  }
  switch(data[i]+colThick[colThick.length-1]) {
   case "11": table += "┤"; break;
   case "12": table += "╢"; break;
   case "21": table += "╡"; break;
   case "22": table += "╣"; break;
  }
  table += "\n"+vertical(data[i+1].split(","));
 }

 table += "\n";
 switch(data[data.length-1]+colThick[0]) {
  case "11": table += "└"; break;
  case "12": table += "╙"; break;
  case "21": table += "╘"; break;
  case "22": table += "╚"; break;
 }

 table += horizontal(cellSize[0], data[data.length-1]);
 for(let i = 1; i < colThick.length-1; i++) {
  switch(data[data.length-1]+colThick[i]) {
   case "11": table += "┴"; break;
   case "12": table += "╨"; break;
   case "21": table += "╧"; break;
   case "22": table += "╩"; break;
  }
  table += horizontal(cellSize[i], data[data.length-1]);
 }
 switch(data[data.length-1]+colThick[colThick.length-1]) {
  case "11": table += "┘"; break;
  case "12": table += "╜"; break;
  case "21": table += "╛"; break;
  case "22": table += "╝"; break;  
 }
 return table;
}

function horizontal(len, thick) {
 let out = "";
 for(let i = 0; i < len; i++) {
  out += (thick == 1) ? "─" : "═";
 }
 return out;
}

function vertical(values) {
 let out = "";
 for(let i = 0; i < colThick.length; i++) {
  out += (colThick[i] == 1) ? "│" : "║";
  if(i != colThick.length-1) {
   const difference = cellSize[i] - values[i].length;
   out += values[i];
   if(difference != 0) {
    for(let j = 0; j < difference; j++) {
     out += " ";
    }
   }
  }
 }
 return out;
}
