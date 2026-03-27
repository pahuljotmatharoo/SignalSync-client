package services

import (
	"database/sql"
	"fmt"
	"signalsync/backend/database"
)

// we can see that in every request we are having to "ValidateLogin", makes more sense to
// use something like JWT token, etc to validate login

func DeleteUserService(db *sql.DB, req *database.SessionVerification) bool {
	login_verification := ValidateAPISessionService(db, req)
	if !login_verification {
		return false
	}
	_, err := db.Exec("DELETE FROM login WHERE username = ?", req.Username)
	fmt.Print(err)
	return err == nil
}
