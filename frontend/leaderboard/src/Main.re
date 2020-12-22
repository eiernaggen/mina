/*
 Main.re is the entry point of the leaderboard project.

 Main.re has the responsibilities for querying the archive postgres database for
 all the blockchain data and parsing the rows into blocks.

 Additionally, Main.re expects to have the credentials, spreadsheet id, and postgres
 connection string available in the form of environment variables.  */

let getEnvOrFail = name =>
  switch (Js.Dict.get(Node.Process.process##env, name)) {
  | Some(value) => value
  | None => failwith({j|Couldn't find env var: `$name`|j})
  };

/* The Google Sheets API expects the credentials to be a local file instead of a parameter
       Thus, we set an environment variable indicating it's path.
   */
Node.Process.putEnvVar(
  "GOOGLE_APPLICATION_CREDENTIALS",
  "./google_sheets_credentials.json",
);

let credentials = getEnvOrFail("GOOGLE_APPLICATION_CREDENTIALS");
let spreadsheetId = getEnvOrFail("SPREADSHEET_ID");
//let pgConnection = getEnvOrFail("PGCONN");

let pgConnectionPreFork = "postgres://postgres:foobar@localhost:5432/archiver_prefork";
let pgConnectionPostFork = "postgres://postgres:foobar@localhost:5432/archiver_postfork";

let main = () => {
  let preforkPool = Postgres.createPool(pgConnectionPreFork);
  let postforkPool = Postgres.createPool(pgConnectionPostFork);
  Js.log("Making prefork query");
  Postgres.makeQuery(preforkPool, Postgres.getLateBlocks, result => {
    switch (result) {
    | Ok(preforkBlocks) =>
      Js.log("Making postfork query");
      Postgres.makeQuery(postforkPool, Postgres.getLateBlocks, result => {
        switch (result) {
        | Ok(postforkBlocks) =>
          let blocks = Belt.Array.concat(preforkBlocks, postforkBlocks);
          Js.log("Before parsing");

          Types.Block.parseBlocks(blocks)
          |> Metrics.calculateMetrics
          |> UploadLeaderboardPoints.uploadChallengePoints(spreadsheetId);

          Postgres.endPool(preforkPool);
          Postgres.endPool(postforkPool);

          Js.log("After upload");
        | Error(error) => Js.log(error)
        }
      });
    | Error(error) => Js.log(error)
    }
  });
  Postgres.makeQuery(postforkPool, Postgres.getBlockHeight, result => {
    switch (result) {
    | Ok(blockHeightQuery) =>
      Belt.Option.(
        Js.Json.(
          blockHeightQuery[0]
          ->decodeObject
          ->flatMap(__x => Js.Dict.get(__x, "max"))
          ->flatMap(decodeString)
          ->mapWithDefault((), height => {
              UploadLeaderboardData.uploadData(spreadsheetId, height)
            })
        )
      )
    | Error(error) => Js.log(error)
    }
  });
  UploadLeaderboardData.uploadUserProfileData(spreadsheetId);
};

main();
