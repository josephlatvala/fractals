let max_iterations = 1500

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

let () =
    for row = 0 to 34 do
        for column = 0 to 70 do

            let c = {
                a = -1.4 +. 0.026 *. (float_of_int column);
                b = -0.85 +. 0.05 *. (float_of_int row);
            } in

            if in_mandelbrot c then
                print_char '#'
            else
                print_char '.'
        done;
        print_char '\n'
    done
