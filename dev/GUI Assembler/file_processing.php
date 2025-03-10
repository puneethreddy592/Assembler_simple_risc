<?php

$text = htmlentities($_POST["textarea"]);
$text = str_replace("\r\n", "\n", $text);
$text = $text . "\n";
$fp = fopen("rani.txt", "w"); // Use "w" instead of "a"
fwrite($fp, $text);
fclose($fp);

// if your device is windows then
// exec('gcc -o main.exe ../main.c');
// $s = shell_exec('.\main');
exec("gcc -o main ../main.c");

$s = shell_exec("./main rani.txt");

// echo $s;

if (file_exists("output.int")) {
    $int = file_get_contents("output.int");
    //  echo "<pre>" . $int . "</pre>";
}
?>
<DOCTYPE html>
    <html>
        <head>
            <title>GUI Assembler</title>
            <meta http-equiv="author" content="sai" />
            <link rel="stylesheet" href="main.css" />
            <link rel="stylesheet" href="normalize.css" />
        </head>
        <body>
            <h1 class="title">Simple Risc Assembler</h1>
            <div class="title-underline"></div>
            <br />
            <small>
                Simple Risc : Documentation is found here :-
                <a
                    href="https://github.com/sowhatnowgithub/Assembler_simple_risc"
                    ,
                    target="_blank"
                    >Github</a
                >
            </small>

            <form action="file_processing.php" method="post" class="form">
                <fieldset>
                    <legend>Code Area</legend>
                    <button class="btn btn-hipster" type="submit">
                        Assemble
                    </button>
                    <label for="textarea" class="form-lable">Textarea</label>
                    <textarea
                        class="form-textarea"
                        name="textarea"
                        spellcheck="false"
                    ><?php echo $text; ?></textarea
                    >
                </fieldset>
            </form>
            <div class="codeoutput">
            <div class="codeoutput bin">
            <?php
            echo $s;
            echo '<small style="color: #ff8906">Binary Output</small>';
            if (file_exists("output.bin")) {
                $bin = file("output.bin");
                foreach ($bin as $b) {
                    echo $b . "<br>";
                }
            }
            ?>
            </div>
            <div class="codeoutput int">
            <?php
            echo '<small style="color: #ff8906">Integer Ouput</small>';
            if (file_exists("output.int")) {
                $int = file("output.int");
                foreach ($int as $b) {
                    echo $b . "<br>";
                }
            }
            ?>
            </div
            </div>
        </body>
    </html>
</DOCTYPE>
