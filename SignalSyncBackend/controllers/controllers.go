package controllers

import (
	"database/sql"
	"net/http"
	"signalsync/backend/database"
	"signalsync/backend/services"

	"github.com/gin-gonic/gin"
)

func ValidateLogin(db *sql.DB) gin.HandlerFunc {
	return func(c *gin.Context) {
		var req database.Login

		if !services.ValidateRequestBodyService(&req, c) {
			return
		}
		err, api_key := services.ValidateLoginService(db, &req)
		if !err {
			c.IndentedJSON(http.StatusBadRequest, gin.H{"error": "Invalid Credentials"})
			return
		}

		c.String(http.StatusOK, api_key)
	}
}

func RegisterUser(db *sql.DB) gin.HandlerFunc {
	return func(c *gin.Context) {
		var req database.Login

		if !services.ValidateRequestBodyService(&req, c) {
			return
		}

		if !services.RegisterUserService(db, &req) {
			c.IndentedJSON(http.StatusBadRequest, gin.H{"error": "Invalid Credentials"})
			return
		}

		c.IndentedJSON(http.StatusOK, gin.H{"success": "Register Successful"})

	}
}

func DeleteUser(db *sql.DB) gin.HandlerFunc {
	return func(c *gin.Context) {
		var req database.Login

		if !services.ValidateRequestBodyService(&req, c) {
			return
		}

		if !services.DeleteUserService(db, &req) {
			c.IndentedJSON(http.StatusBadRequest, gin.H{"error": "Invalid Credentials"})
			return
		}

		c.IndentedJSON(http.StatusOK, gin.H{"success": "Delete Successful"})

	}
}

func ValidateAPIKey(db *sql.DB) gin.HandlerFunc {
	return func(c *gin.Context) {
		var req database.SessionVerification

		if !services.ValidateRequestBodyService(&req, c) {
			return
		}

		if !services.ValidateAPISessionService(db, &req) {
			c.IndentedJSON(http.StatusBadRequest, gin.H{"error": "Invalid Credentials"})
			return
		}
		c.IndentedJSON(http.StatusOK, gin.H{"success": "Delete Successful"})
	}
}
