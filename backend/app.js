import express from 'express';
import { dbConnection } from './configuration/db_access.js';
import{config as dotenvConfig} from 'dotenv';
import fs from 'fs';

const app = express();

const PORT = process.env.PORT || 5000;

const secretPath =
fs.existsSync('/etc/secret/.env')
? '/etc/secret/.env'
:'./.env';

dotenvConfig({path : secretPath});


app.get("/", (req, res)=>{
    res.json({message: "server is working!"});
});

app.listen(PORT, ()=>{
    dbConnection();
     console.log("server started at http://localhost:=+PORT");
});
