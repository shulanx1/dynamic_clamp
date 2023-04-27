function upload_to_teensy(out, port)
out = single(out);
out = typecast(out, 'uint8');


% Send the numbers to the Teensy microcontroller.
fwrite(port, out);

end