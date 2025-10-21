let max_iterations = 3000

type complex = {a: float; b: float}


(* Complex number type 
 * needs `compare` so it can be used in a set
 *)
module Complex = struct
        type t = complex
        let compare x y =
            if x.a > y.a && x.b > y.b then 1
            else if x = y then 0
            else -1
end

(* Set of complex numbers *)
module ComplexSet = Set.Make(Complex)

(* iterate mandelbrot function `z := z^2 + c`
 * Returns true if it goes into a cycle
 * Returns false if it exceeds `max_iterations` (likely diverges)
 *)
let rec iterate (c: complex) (z: complex)
    (iterations: int) (past_numbers: ComplexSet.t) : bool =

    let new_z: complex = {
        a = (z.a *. z.a) -. (z.b *. z.b) +. c.a;
        b = (2. *. z.a *. z.b) +. c.b;
    } in

    match iterations with
    | 0 -> false
    | _ -> if ComplexSet.mem new_z past_numbers
        then true
        else iterate c new_z
            (iterations - 1)
            (ComplexSet.add new_z past_numbers)


(* Checks if a value c is in the mandelbrot set *)
let in_mandelbrot (c: complex) : bool =
    iterate c {a=0.; b=0.} max_iterations ComplexSet.empty

let in_julia (c: complex) (z: complex) : bool =
    iterate c z max_iterations ComplexSet.empty

let () =
    let in_set =
        if Array.length Sys.argv >= 3 then
            try let c = {
                    a = Stdlib.float_of_string Sys.argv.(1);
                    b = Stdlib.float_of_string Sys.argv.(2);
                } in
                in_julia c
            with Failure _ -> in_mandelbrot
        else
            in_mandelbrot
    in

    for row = 0 to 47 do
        for column = 0 to 110 do

            let number = {
                a = -1.4 +. 0.026 *. (float_of_int column);
                b = -1.25 +. 0.05 *. (float_of_int row);
            } in

            if in_set number then
                print_char '#'
            else
                print_char '.'
        done;
        print_char '\n'
    done
