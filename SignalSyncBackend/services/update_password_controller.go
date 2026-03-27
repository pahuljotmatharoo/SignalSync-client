package services

import (
	"database/sql"
	"fmt"
	"signalsync/backend/database"
)

// we can see that in every request we are having to "ValidateLogin", makes more sense to
// use something like JWT token, etc to validate login

func UpdatePasswordService(db *sql.DB, req *database.Login) bool {
	var session_obj database.SessionVerification
	session_obj.Api_key = req.Api_key
	session_obj.Username = req.Username
	login_verification := ValidateAPISessionService(db, &session_obj)
	if !login_verification {
		return false
	}
	_, err := db.Exec("UPDATE login SET password = ? WHERE username = ?", req.Password, req.Username)
	fmt.Print(err)
	return err == nil
}
