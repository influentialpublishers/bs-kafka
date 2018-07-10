exception DynamicCastError;

%bs.raw {|
function myinstanceof(c, x) { return x instanceof c }
|};

[@bs.val]
external instanceof :
  ([@bs.unwrap] [ | `c('c)], [@bs.unwrap] [ | `x('x)]) => bool =
  "myinstanceof";

let instanceof = (c, x) => instanceof(`c(c), `x(x));

let bufferConstructor = [%bs.raw "Buffer"];

let instanceofBuffer = x => instanceof(bufferConstructor, x);

let maybeBuffer = x => x |> instanceofBuffer ? Some(Obj.magic(x) : Node.buffer) : None;

let bufferGetExn = x => x |> instanceofBuffer ? (Obj.magic(x) : Node.buffer) : raise(DynamicCastError);
